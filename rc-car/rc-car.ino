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
float rpm[] = {0,0,0,0};

// rc control inputs; array structure_: {CH3, accel, steer}
const int RC[] = {14,15,16};
int duration[] = {0,0,0};
const int duration_max = 2041; //microseconds
const int duration_min = 1041; //microseconds

# define STEER 3

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

  //Serial.begin(115200);
  radio.begin(); // Start the radio
  radio.openWritingPipe(address); // Set the address
  radio.setPALevel(RF24_PA_MIN); // Set the power level
  radio.stopListening(); // Stop listening for incoming messages

  set_interrupts();
}

void loop() 
{
  /*
  // read inputs from RC control
  for(int i=0; i<3; i++)
  {
    duration[i] = pulseIn(RC[i], HIGH, 20000);
    Serial.print(duration[i]," ");
  }
  */

  // calculate speeds
  for(int i=0; i<4; i++)
  {
    rpm[i] = (60.0*(1000))/T_rpm[i];
    //Serial.print(digitalRead(RPM[i]));
    //Serial.print(" ");
    Serial.print(rpm[i]);
    Serial.print(" ");
  }
  snprintf(msg, ,"%f4.2, %f4.2, %f4.2, %f4.2", rpm[0], rpm[1], rpm[2], rpm[3])
  radio.write(msg,sizeof(msg));

  int rpm_motor = 5*0.5*(abs(rpm[2] - rpm[3]));
  float v_long = (3.1416 * 0.0686 * 0.5 * (rpm[0] + rpm[1]))/60; // pi*d*0.5(rpm_fl + rpm_fr)

  delay(100);
}






