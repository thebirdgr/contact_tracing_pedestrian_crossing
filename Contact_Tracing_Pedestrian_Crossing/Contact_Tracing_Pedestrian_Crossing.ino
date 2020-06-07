/*
  Contact Tracing Pedestrian Crossing IoT Prototype

  COVID-19 Project

  This code listens for bluetooth devices that are near the signal and makes "handshakes" with the devices, exchanging a temporary id between the devices 
  and reading them.

  Besure to include the necessary libraries. 

  The database is powered by MongoDB and the server side Communication supported my Express.

  As of now, the central device is yet to send a temporary id to the IoT device and the full temporary id is yet to be displayed.
*/

#include <ArduinoBLE.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Arduino_LSM6DS3.h>
#include "utility/wifi_drv.h"
#include "arduino_secrets.h"

// bluetooth service config
BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214");    // BLE LED Service
BLEService accelerometer("38ac34a6-9f32-11ea-bb37-0242ac130002"); // Accelerometer Service
BLEService tempid("d02aa34c-a3df-11ea-bb37-0242ac130002");        // temporary ID Service

// BLE LED Switch Characteristic - custom 128-bit UUID, readable, writable or notifiable by central
BLEByteCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
BLEStringCharacteristic accelerometerCharacteristic("38ac34a6-9f32-11ea-bb37-0242ac130002", BLERead | BLEWrite | BLENotify, 10);
BLEStringCharacteristic tempidCharacteristic("d02aa34c-a3df-11ea-bb37-0242ac130002",BLERead | BLEWrite | BLENotify, 18);


const int irPin = 2;        // Pin that reads the IR sensor
char orientation[30] = {0}; //  initialising array to zero
int loopCount = 0;
unsigned long starttime, endtime;
//char c = "";
String stringOne = "";      // will be used to store temporary id

// please enter your sensitive data in the Secret tab/arduino_secrets.h
// you wiil need your own #include "arduino_secrets.h"
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;


// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

// numeric IP for my local server (no DNS) fill up 'x' values
IPAddress server(xxx,xxx,xxx,xxx);  
int status = WL_IDLE_STATUS;
bool networkInitialized = false;
bool wifiModeFlag = true;  //true because we want to start with wifi first

const int LED_PIN = LED_BUILTIN; 

void setup()
{
  Serial.begin( 9600 );
  while ( !Serial );

  // add for ir blaster
  pinMode(irPin, INPUT);
  // for the builtin LED of Arduino board
  pinMode(13, OUTPUT);
  //check for frimware
  checkStatusAndFirmware();
  //start accelerometer
  // 3-axis sensor initialization
  startAccelerometer();
}


void loop()
{
  if( !networkInitialized )
  {
    if( !wifiModeFlag )
    {
      Serial.print( "Switch to BLE: " );
      if( !switch2BleMode() )
      {
        Serial.println( "failed" );
      }
      else
      {
        networkInitialized = true;
        Serial.println( "success" );
      }
    }
    else
    {
      Serial.print( "Switch to WiFi: " );
      if( !switch2WiFiMode() )
      {
        Serial.println( "failed" );
      }
      else
      {
        networkInitialized = true;
        Serial.println( "success" );
      }
    }
  }
  else
  {
    if( !wifiModeFlag )
    {
      bleMode();
    }
    else
    {
      wifiMode();

      // if there are incoming bytes available
      // from the server, read them and print them:
      while(client.connected()){
        readWebsite(); // this function had to be in some sort of loop to be able to read all the characters, so a while loop
      }

      networkInitialized = false;
      wifiModeFlag = false;
    
      // stop the client:
      disconnected();
    }
  }
}


void bleMode(){ 
  // these values are for calculating the orientation using built-in accelerometer (only for testing purposes)
  float x, y, z, delta = 0.05;
  
  if(!digitalRead(irPin)){
    // connect to devices only if motion is detected
    Serial.println("Signal Pressed.");
    // code here
    connect2Client(x, y, z, delta); //(only for testing purposes)
  }
}


void wifiMode() 
{
  // connects to your wifi and to any server that you declare, default server for localhost is port 80
  connect2WifiAnd2Server();
}

bool switch2BleMode()  
{
  // Bluetooth initialization
  if ( !BLE.begin() )
  {
    Serial.println("starting BLE failed!");
    return false;
  }

  // set advertised local name and service UUIDs:
  BLE.setLocalName("Accelerometer & Temporary ID");
  BLE.setAdvertisedService(ledService);
  BLE.setAdvertisedService(accelerometer);
  BLE.setAdvertisedService(tempid);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);
  accelerometer.addCharacteristic(accelerometerCharacteristic);
  tempid.addCharacteristic(tempidCharacteristic);

  // add services
  BLE.addService(ledService);
  BLE.addService(accelerometer);
  BLE.addService(tempid);

  // set the initial value for the characeristics:
  switchCharacteristic.writeValue(0);
  accelerometerCharacteristic.writeValue(orientation);
  Serial.print(stringOne);  // for testing purposes
  tempidCharacteristic.writeValue(stringOne);
  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");

  return true;
}


bool switch2WiFiMode()
{
  BLE.stopAdvertise();
  BLE.end();

  status = WL_IDLE_STATUS;

  // Re-initialize the WiFi driver
  // This is currently necessary to switch from BLE to WiFi
  wiFiDrv.wifiDriverDeinit();
  wiFiDrv.wifiDriverInit();

  return true;
}

void printWiFiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void startAccelerometer(){
  // this code is to initialize the accelerometer, however it is not necessary for Contact Tracing
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
        // loopCount = loopCount+1; a device was connected, you can use this to keep track of how many devices were connected before the signal changes 
        // for Pedestrians to walk

        Serial.print("Connected to central: ");
        // print the central's MAC address:
        Serial.println(central.address());
        
        long currentMillis = millis();
        long timeoutMillis = currentMillis + 10000; //10 second timeout, can be shorter if there was a proprietary app like "TraceTogether", however I 
        // used nrf Connect for testing and my reactions were slow
        
        // while the central is still connected to peripheral:
        while (central.connected()){
          currentMillis = millis();
          // calculate board orientation and then add it to accelerometer characteristic
          checkOrientation(x, y, z, delta);
          if (timeoutMillis - currentMillis <= 0) {
              Serial.println("Timeout");
              central.disconnect();
          }  
        }

        // change flag to switch to wifi mode
        networkInitialized = false;
        wifiModeFlag = true;

        // when the central disconnects, print it out:
        Serial.print(("Disconnected from central: "));
        Serial.println(central.address());
        return;
      }
}

void checkOrientation(float x, float y, float z, float delta){
  // this code will return the orientationof the arduino board, note that this is only for testing purposes
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

void checkStatusAndFirmware(){
  if (WiFi.status() == WL_NO_MODULE){
    Serial.println("Communication with WiFi module failed!");
    // don't continue, I'm not sure what this means but it's included in the docs but it is necessary for your baord to be compatible with the version
    // of the WiFi library that you plan to use
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION){
    Serial.println("Please upgrade the firmware");
  }
}


void connect2WifiAnd2Server(){
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWiFiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /");
    client.println("Host: 192.168.1.109:80");
    client.println("Connection: close");
    client.println();
  }
}

void readWebsite(){
  while (client.available()){
    char c = client.read();
    stringOne = String(c).substring(0, 84); 
    Serial.print(stringOne);
  }
}

void disconnected(){
    Serial.println();
    Serial.println("disconnecting from server.");
    client.stop(); // force stops the wifi connection
}
