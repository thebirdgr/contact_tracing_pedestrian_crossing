# Contact Tracing Pedestrian Crossing

> This is only a prototype and has a flaw on displaying data, which I will get into later. Nevertheless this project is opensource, so feel free to use it as a base for your project.

### Purpose and Use

This project was created in view of COVID-19. It is used to make note of people who were crowded at the crossing while waiting for the signal. This helps in delayed contact tracing where the virus may still be in the near vicinity of that crossing, to inform future users
It is also touch-free as it uses and IR Sensor instead of a traditional button, so the public and feel comfortable using the signal.
It's supposed to have a public health system develop the temporary id that is needed to be exchanged, however, in this example I've used a random alpha numeric string of 84 bytes.

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

