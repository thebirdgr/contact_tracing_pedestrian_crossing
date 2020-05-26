#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>


BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service
BLEService accelerometer("38ac34a6-9f32-11ea-bb37-0242ac130002"); // Accelerometer Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLECharCharacteristic accelerometerCharacteristic("38ac34a6-9f32-11ea-bb37-0242ac130002", BLERead | BLENotify);

const int irPin = 2;

void setup()
{
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  pinMode(13, OUTPUT);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  // 3-axis sensor initialization
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

    // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);
  BLE.setAdvertisedService(accelerometer);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  accelerometer.addCharacteristic(accelerometerCharacteristic);

  // add service
  BLE.addService(ledService);
  BLE.addService(acclerometer);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");

}

void loop(){
  float x, y, z, delta = 0.05;
  
  if(digitalRead(irPin)){
    digitalWrite(13, LOW);
  }
  else{
        Serial.println("Motion Detected");
    // connect to devices only if motion is detected
    // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
     Serial.println(central.address());
     //    digitalWrite(13, HIGH);   
     
      // while the central is still connected to peripheral:
    while (central.connected()) {    //instead of central.connected() change to irPin value
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
          Serial.println("LED on");
          digitalWrite(13, HIGH);         // will turn the LED on
        } else {                              // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(13, LOW);          // will turn the LED off
        }
      }
    }

    // when the central disconnects, print it out:
    Serial.print(("Disconnected from central: "));
    Serial.println(central.address());
    } 
  }
}
