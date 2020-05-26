#include <Servo.h>


const int SensorPin = 3;  
//int sensorValue = 0;                
int myservo = 9;
int pos = 0; 
Servo servo;

void setup() {
  
servo.attach(myservo);
}

void loop() {
  
  digitalRead(3);
  if(SensorPin==HIGH) 
  servo.attach(9);  
  pos = 90;               // start at midpoint 90 degrees
//  Speed = 3;              // servo moves 3 degrees each time left/right is pushed// attaches the servo on pin 9 to the servo object 
             // start at midpoint 90 degrees             // servo moves 3 degrees each time left/right is pushed    // Set initial position
             
}