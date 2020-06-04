#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 

char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(192,168,1,109);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");    // BLE LED Service
BLEService accelerometer("38ac34a6-9f32-11ea-bb37-0242ac130002"); // Accelerometer Service
// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEStringCharacteristic accelerometerCharacteristic("38ac34a6-9f32-11ea-bb37-0242ac130002", BLERead | BLEWrite | BLENotify, 10);

const int irPin = 2;
char orientation[30] = {0}; //initializing array to zero

void setup()
{
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  pinMode(13, OUTPUT);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()){
    Serial.println("starting BLE failed!");

    while (1)
      ;
  }
  // 3-axis sensor initialization
  if (!IMU.begin())
  {
    Serial.println("Failed to initialize IMU!");

    while (1)
      ;
  }
  Serial.print("Accelerometer sample rate = ");
  Serial.print(IMU.accelerationSampleRate());
  Serial.println(" Hz");
  Serial.println();
  Serial.println("Acceleration in G's");
  Serial.println("X\tY\tZ");

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);
  BLE.setAdvertisedService(accelerometer);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  accelerometer.addCharacteristic(accelerometerCharacteristic);

  // add service
  BLE.addService(ledService);
  BLE.addService(accelerometer);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
  accelerometerCharacteristic.writeValue(orientation);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop()
{
  float x, y, z, delta = 0.05;

  if (digitalRead(irPin))
  {
    digitalWrite(13, LOW);
  }
  else
  {
    Serial.println("Motion Detected");
    // connect to devices only if motion is detected
    // listen for BLE peripherals to connect:
    BLEDevice central = BLE.central();

    // if a central is connected to peripheral:
    if (central)
    {

      Serial.print("Connected to central: ");
      // print the central's MAC address:
      Serial.println(central.address());
      //    digitalWrite(13, HIGH);

      // while the central is still connected to peripheral:
      while (central.connected())
      {

        //accelerometerCharacteristic.value(checkOrientation(x, y, z));
        if (IMU.accelerationAvailable())
        {
          IMU.readAcceleration(x, y, z);

          if (y <= delta && y >= -delta)
            strcpy(orientation, "flat");
          else if (y > delta && y < 1 - delta)
            strcpy(orientation, "tilted to the left");
          else if (y >= 1 - delta)
            strcpy(orientation, "left");
          else if (y < -delta && y > delta - 1)
            strcpy(orientation, "tilted to the right");
          else
            strcpy(orientation, "right");

          accelerometerCharacteristic.writeValue(orientation);

        }

        // if the remote device wrote to the characteristic,
        // use the value to control the LED:
        if (switchCharacteristic.written())
        {
          if (switchCharacteristic.value())
          { // any value other than 0
            Serial.println("LED on");
            digitalWrite(13, HIGH); // will turn the LED on
          }
          else
          { // a 0 value
            Serial.println(F("LED off"));
            digitalWrite(13, LOW); // will turn the LED off
          }
        }
      }
      //      }else{
      //        BLE.disconnect();
      //      }

      // when the central disconnects, print it out:
      Serial.print(("Disconnected from central: "));
      Serial.println(central.address());
    }
  }
}
