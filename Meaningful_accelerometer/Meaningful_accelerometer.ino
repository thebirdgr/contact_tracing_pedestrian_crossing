/*
  Arduino LSM6DS3 - Simple Accelerometer

  This example reads the acceleration values from the LSM6DS3
  sensor and continuously prints them to the Serial Monitor
  or Serial Plotter.

  The circuit:
  - Arduino Uno WiFi Rev 2 or Arduino Nano 33 IoT

  created 10 Jul 2019
  by Riccardo Rizzo

  This example code is in the public domain.
*/

#include <Arduino_LSM6DS3.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");
}
void loop() {
  // https://maker.pro/arduino/tutorial/how-to-use-the-arduino-nano-33-bles-built-in-imu
  
  float x, y, z, delta = 0.05;

  if (IMU.accelerationAvailable())
  {
    IMU.readAcceleration(x, y, z);
 
    if(y <= delta && y >= -delta)
          Serial.println("flat");
    else if(y > delta && y < 1 - delta)
          Serial.println("tilted to the left");
    else if(y >= 1 - delta)
          Serial.println("left");
    else if(y < -delta && y > delta - 1)
          Serial.println("tilted to the right");
    else
          Serial.println("right");
}

}
