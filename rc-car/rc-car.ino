/*
Code for the PC-connected Arduino.
This Arduino reads the values from the RC control and sends them to the PC.
It obtains the motor control values and sents them over the NRF24L01 radio control to the vehicle.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// rpm measurement; array structure: {FL, FR, RL, RR}
const int RPM[] = {19,2,4,7}; // PIN numbers
long T_rpm[] = {2147483647, 2147483647, 2147483647, 2147483647}; // init with infinite values
long starttime[] = {0,0,0,0};
bool sema[] = {1,1,1,1};
int rpm[] = {0,0,0,0};

// rc control inputs; array structure_: {steer, accel, CH3}
const int RC[] = {14,15,16};
int duration[] = {0,0,0};
const int duration_max = 2041; //microseconds
const int duration_min = 1041; //microseconds

// pins for output control
# define STEER 3 // pwm pin

# define FORWARD 5 // pwm pin
# define REVERSE 6 // pwm pin

int launch_state = 0; // 0: idle, 1: activated, 2: ready (car in standstill), 3: during launch

RF24 radio(9, 8); // CE and CSN pins
const byte address[6] = "00001"; // Address for communication

float v_long = 0;

void setup()
{  
  // set rpm pins to input mode
  for(int i=0; i<4; i++)
  {
    pinMode(RPM[i], INPUT);
  }

  // set RC input pins to input mode
  for(int i=0; i<3; i++)
  {
    pinMode(RC[i], INPUT);
  }

  // set output pins
  pinMode(STEER, OUTPUT);
  pinMode(FORWARD, OUTPUT);
  pinMode(REVERSE, OUTPUT);

  // start Serial and RF24 connection
  Serial.begin(115200);
  radio.begin(); // Start the radio
  radio.openWritingPipe(address); // Set the address
  radio.setPALevel(RF24_PA_MIN); // Set the power level
  radio.stopListening(); // Stop listening for incoming messages

  // activate interrupts for rpm measurment
  set_interrupts();
}

void loop() 
{  
  // calculate speeds
  for(int i=0; i<4; i++)
  {
    rpm[i] = (60.0*(1000))/T_rpm[i];
  }
  int rpm_motor = 5*0.5*(abs(rpm[2] - rpm[3]));
  float v_long = (3.1416 * 0.0686 * 0.5 * (rpm[0] + rpm[1]))/60; // pi*d*0.5(rpm_fl + rpm_fr)

  // send speeds over RF24 radio to PC logger
  char msg[24];
  sprintf(msg, "%d, %d, %d, %d", rpm[0], rpm[1], rpm[2], rpm[3]);
  radio.write(msg,sizeof(msg));
  // read inputs from RC control
  for(int i=0; i<3; i++)
  {
    duration[i] = pulseIn(RC[i], HIGH, 20000);
    delay(10);
  }

  // set output steering
  int steer_pwm = transfer_steer(duration[0]);
  analogWrite(STEER, steer_pwm);

  // acceleration / launch control state machine
  int forward_pwm = 0;
  int reverse_pwm = 0;
  switch(launch_state) // idle
  {
    case 0: // idle
      forward_pwm = transfer_accel(duration[1], 1);
      reverse_pwm = transfer_accel(duration[1], 0);
      //transition condition (button press activates launch control)
      if(duration[2] > duration_max*0.95)
      {
        launch_state = 1;
      }
      break;
  
    case 1: // activated (waiting for standstill)
      forward_pwm = transfer_accel(duration[1], 1);
      reverse_pwm = transfer_accel(duration[1], 0);
      //transition condition (waiting for standstill)
      if(abs(v_long) < 0.01)
      {
         launch_state = 2;
      }
      break;

    case 2: // ready (standstill, waiting for launch)
      forward_pwm  = 0;
      reverse_pwm = 0;
      // transition condition to start launc
      if(duration[1] > duration_max*0.95)
      {
        launch_state = 3;
      }
      break;

    case 3: // during launch
      forward_pwm = launch(rpm);
      reverse_pwm = 0;
      // transition condition to end launch control
      if (duration[1] < duration_max*0.95)
      {
        launch_state = 0;
      }
      break;
  }

  // set output accel 
  analogWrite(FORWARD, forward_pwm);
  analogWrite(REVERSE, reverse_pwm);
}

