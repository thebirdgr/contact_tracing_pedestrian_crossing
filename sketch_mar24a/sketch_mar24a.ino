#include <Servo.h>

#define trig_pin 13
#define echo_pin 12

#define motor_channel_1a 5
#define motor_channel_1b 6

#define lifty_pin 3
#define lifty_wheel_a_pin 10
#define lifty_wheel_b_pin 11

Servo lifty;
Servo lifty_wheel_a;
Servo lifty_wheel_b;

long get_distance() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);
  return 0.01725 * pulseIn(echo_pin, HIGH);
}

void lifty_control(int angle) {
  lifty.write(angle);
}

void lifty_wheel_control(int speed) {
  lifty_wheel_a.writeMicroseconds(1500 + speed);
  lifty_wheel_b.writeMicroseconds(1500 - speed);
}

void move_forward(int speed) {
  analogWrite(motor_channel_1a, 0);
  analogWrite(motor_channel_1b, speed);
}

void setup() {
  Serial.begin(9600);
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  
  lifty.attach(lifty_pin);
  lifty_wheel_a.attach(lifty_wheel_a_pin);
  lifty_wheel_b.attach(lifty_wheel_b_pin);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  long cur_dist = get_distance();
  Serial.println(cur_dist);
  if (cur_dist <= 5) {
    move_forward(0);
    lifty_control(0);
    lifty_wheel_control(0);
    lifty_control90);
  } else {
    move_forward(127);
    lifty_wheel_control(200); 
  }
  delayMicroseconds(1000);

}
