/*
Code for the PC-connected Arduino.
This Arduino reads the values from the RC control and sends them to the PC.
It obtains the motor control values and sents them over the NRF24L01 radio control to the vehicle.
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <Servo.h>

// rpm measurement, array structure: {FL, FR, RL, RR}
const int RPM = {2,3,4,5} // PIN numbers
long T[] = {2147483647, 2147483647, 2147483647, 2147483647}; // init with infinite values
long starttime[] = {0,0,0,0};
bool sema[] = {0,0,0,0};
float rpm[] = {0,0,0,0};

#define STEER 7 // output for steering 

RF24 radio(7, 8); // CE and CSN pins
const byte address[6] = "00001"; // Address for communication

const int duration_max = 2041; //microseconds
const int duration_min = 1041; //microseconds

float v_long = 0;

void setup()
{  
  // set rpm pins to input mode
  for(int i=0; i<4; i++)
  {
    pinMode(RPM[i], INPUT);
  }

  Serial.begin(115200);
  // radio.begin(); // Start the radio
  // radio.openWritingPipe(address); // Set the address
  // radio.setPALevel(RF24_PA_MIN); // Set the power level
  // radio.stopListening(); // Stop listening for incoming messages

  set_interrupts();
}

void loop() 
{
  // i++;
  // // read input from RC control
  // int duration_accel = pulseIn(CH3, HIGH, 20000); // acceleration
  // int accel = map(duration_accel, duration_min, duration_max, 0, 255);

  // int duration_steer = pulseIn(CH4, HIGH, 20000); // steering
  // int steer = map(duration_steer, duration_min, duration_max, 0, 255);

  // Serial.print(duration_accel);
  // Serial.print(" ");
  // Serial.print(accel);
  // Serial.print(" ");
  // Serial.print(duration_steer);
  // Serial.print(" ");
  // Serial.print(steer);
  // Serial.print(" ");
  // Serial.println(i);

  // delay(100);

  // calculate speeds
  for(int i=0; i<4; i++)
  {
    rpm[i] = (60.0*10^6)/T;
    Serial.print(rpm[i]," ") 
  }
  v_long = 3.1416 * 0.0686 * 0.5 * (rpm[0] + rpm[1]); // pi*d*0.5(rpm_fl + rpm_fr)
  Serial.println(v_long)
}






