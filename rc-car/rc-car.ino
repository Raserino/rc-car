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
  // read inputs from RC control
  for(int i=0; i<3; i++)
  {
    duration[i] = pulseIn(RC[i], HIGH, 20000);
    delay(10);
  }
  Serial.println(millis());

  // set steering
  int steer_pwm = transfer_steer(duration[0]);
  analogWrite(STEER, steer_pwm);

  // set acceleration
  int forward_pwm = transfer_accel(duration[1],0);
  analogWrite(FORWARD, forward_pwm);
  int reverse_pwm = transfer_accel(duration[1],1);
  analogWrite(REVERSE, reverse_pwm);

  // calculate speeds
  for(int i=0; i<4; i++)
  {
    rpm[i] = (60.0*(1000))/T_rpm[i];
  }
  int rpm_motor = 5*0.5*(abs(rpm[2] - rpm[3]));
  float v_long = (3.1416 * 0.0686 * 0.5 * (rpm[0] + rpm[1]))/60; // pi*d*0.5(rpm_fl + rpm_fr)

  // send speeds
  char msg[24];
  sprintf(msg, "%d, %d, %d, %d", rpm[0], rpm[1], rpm[2], rpm[3]);
  radio.write(msg,sizeof(msg));
}

