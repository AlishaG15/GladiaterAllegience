xx#include <Arduino.h>

#include <ESP32Servo.h>

Servo myservo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 90;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

int ledPin = GPIO_NUM_6;
int servoPin = GPIO_NUM_2;

void setup() {
  Serial.begin(9600);

	// Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object
	// using default min/max of 1000us and 2000us
	// different servos may require different min/max settings
	// for an accurate 0 to 180 sweep

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  myservo.write(104);

}

void loop() {

	// for (pos = 180; pos >= 180; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(5);             // waits 15ms for the servo to reach the position
	// }

  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	myservo.write(pos/10);    // tell servo to go to position in variable 'pos'
	// 	delay(10);             // waits 15ms for the servo to reach the position
	// }
  delay(5000);
  myservo.write(72);
  Serial.println("Closed degrees");
  delay(10000);
  myservo.write(104);
  Serial.println("Open degrees");
  delay(2000);
  // myservo.write(90);
  // delay(1000);
  // myservo.write(180);
  // delay(1000);
  
  digitalWrite(ledPin, HIGH);
  // Serial.println("I'm alive");
  // delay(500);
}