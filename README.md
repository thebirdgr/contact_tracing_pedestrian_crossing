# Contact Tracing Pedestrian Crossing

> This is only a prototype and has a flaw on displaying data, which I will get into later. Nevertheless this project is opensource, so feel free to use it as a base for your project.

### Purpose and Use

This project was created in view of COVID-19. It is used to make note of people who were standing at the crossing while waiting for the signal. This helps in delayed contact tracing where the virus may still be in the near vicinity of that crossing, to inform future users.
It is also contactless as it uses an IR Sensor instead of a traditional button, so the public can feel comfortable using the signal.
It's supposed to have a public health system that develops the temporary id that is needed to be exchanged, however, in this example I've used a random alpha numeric string of 84 bytes.

It is to be noted that the no data of the user such as location is being collected. Privacy and security was kept in mind while making the device work.
### Hardware 

* Arduino Nano 33 IoT
* Infrared Sensor
* 3 Jumper wires

### Software 

* Arduino IDE
* Database - MongoDB, Mongoose
* Website - Embedded Javascript
* Server - Express.js

### Electrical Connections

The connections are not complicated. I've included an image of how the connections to the board and IR Sensor are to be made.

(https://github.com/thebirdgr/contact_tracing_pedestrian_crossing/blob/master/img/IRConnection.png)

Here's the link to the TinkerCAD.

(https://www.tinkercad.com/things/jpFZ34tKhTA-faucet-draft/editel)


### Flaw

 HTTP/1.1 200 OK
 X-Powered-By: Express
 Content-Type: text/html; charset=utf-8
 Content-Length: 100
 ETag: W/"64-i3GeS7+obc78q/I/9T2hLFCKBMM"
 Date: Thu, 04 Jun 2020 06:01:07 GMT
 Connection: close
 
 When receiving the data from the website, I get this extra information along with the 84 characters. I'm not sure how to remove it to share only the tempid. I'm sure there's a better way to do it like directly talking to my server but I'm not sure how. If anyone can fix it, that'd be great! 

 ### Suggestion

 The primary focus of this device is to work with an app so that communication and transfer of data would be more seamless.  However, I'm not an app developer. So app developers can tinker with this to help 'harmonize' the ecosystem. 

 One more thing, since at least 5 people would be standing at a signal in a working district, it would be best to store atleast 5 tempids in the local IoT signal so as not to switch between wifi and bluetooth continuously or when unable to get internet connection. Arduino has EEPROM to store, but the process is much tedious for me. Hence, local tempid storage would need to be implemented as it is quite necessary.

### References

Here are some sites I found code that assisted me in building the project:

Switch between BLE and Wifi:

(https://forum.arduino.cc/index.php?topic=657710.0)

Accelerometer Orientation:

https://maker.pro/arduino/tutorial/how-to-use-the-arduino-nano-33-bles-built-in-imu

### END

The rest is pretty much software and should work right out of the box if you have the necessary software installed. I hope this project is helpful to anyone looking to contribute in a much more scaled and sophisticated way or even to hobbyists =).
