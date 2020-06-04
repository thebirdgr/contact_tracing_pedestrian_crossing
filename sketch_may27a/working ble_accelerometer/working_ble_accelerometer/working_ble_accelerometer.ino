#include <ArduinoBLE.h>
#include <Arduino_LSM6DS3.h>

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");    // BLE LED Service
BLEService accelerometer("38ac34a6-9f32-11ea-bb37-0242ac130002"); // Accelerometer Service
BLEService tempid("d02aa34c-a3df-11ea-bb37-0242ac130002");        // temporary ID Service

// BLE LED Switch Characteristic - custom 128-bit UUID, readable, writable or notifiable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEStringCharacteristic accelerometerCharacteristic("38ac34a6-9f32-11ea-bb37-0242ac130002", BLERead | BLEWrite | BLENotify, 10);
BLEStringCharacteristic tempidCharacteristic("d02aa34c-a3df-11ea-bb37-0242ac130002",BLERead | BLEWrite | BLENotify, 18);

const int irPin = 2;
char orientation[30] = {0}; //initializing array to zero
int loopCount = 0;
unsigned long starttime, endtime;

void setup()
{
  Serial.begin(9600);
  pinMode(irPin, INPUT);
  pinMode(13, OUTPUT);
  while (!Serial);

  // begin bluetooth initialization
  startBle();
  
  // 3-axis sensor initialization
  startAccelerometer();
}

void loop(){
  loopCount = 0;
  float x, y, z, delta = 0.05;

  if(!digitalRead(irPin)){
    // connect to devices only if motion is detected
    Serial.println("Signal Pressed.");
    // code here
    connect2Client(x, y, z, delta);
    Serial.print(loopCount,DEC); // shows how many devices were connected
  }
}








void startBle(){ // initializes BLE device
  if (!BLE.begin()){
    Serial.println("starting BLE failed!");
    while (1);
  }
  // set advertised local name and service UUID:
  BLE.setLocalName("Accelerometer & Temporary ID");
  BLE.setAdvertisedService(ledService);
  BLE.setAdvertisedService(accelerometer);
  BLE.setAdvertisedService(tempid);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  accelerometer.addCharacteristic(accelerometerCharacteristic);
  tempid.addCharacteristic(tempidCharacteristic);

  // add service
  BLE.addService(ledService);
  BLE.addService(accelerometer);
  BLE.addService(tempid);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);
  accelerometerCharacteristic.writeValue(orientation);
  tempidCharacteristic.writeValue("K{_!pKxYSW:0x3x#^ER7?>g08mldi2B5CM^|sP,J<?1:uOPd~4L1=/U)&FY*-KvF$d[ao0`w|%S3_TF|Yg_(");

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}


void startAccelerometer(){
    if (!IMU.begin()){
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

void connect2Client(float x, float y, float z, float delta){
  // listen for BLE peripherals to connect:
      BLEDevice central = BLE.central();

      // if a central is connected to peripheral:
      if(central){
        loopCount = loopCount+1; //a device was connected
        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());
        
        long currentMillis = millis();
        long timeoutMillis = currentMillis + 10000; //10 second timeout
        
        // while the central is still connected to peripheral:
        while (central.connected()){
          currentMillis = millis();
          // calculate board orientation and then add it to accelerometer characteristic
          checkOrientation(x, y, z, delta);
          if (timeoutMillis - currentMillis <= 0) {
              Serial.println("Timeout");
              loopCount++;
              central.disconnect();
          }  
        }

        // when the central disconnects, print it out:
        Serial.print(("Disconnected from central: "));
        Serial.println(central.address());
      }
}

void checkOrientation(float x, float y, float z, float delta){
  if(IMU.accelerationAvailable()){
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
}

// if the remote device wrote to the characteristic,
// use the value to control the LED:
// controlLED();
void controlLED(){
  if (switchCharacteristic.written()){
          if (switchCharacteristic.value()){ 
            // any value other than 0
            Serial.println("LED on");
            digitalWrite(13, HIGH); // will turn the LED on
          }
          else{
            // a 0 value
            Serial.println(("LED off"));
            digitalWrite(13, LOW); // will turn the LED off
          }
  }
}
