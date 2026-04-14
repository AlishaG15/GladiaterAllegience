#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <ESP32Servo.h>


#define CE_PIN D1
#define CSN_PIN D0

#define LED D5

#define BUTTON D3

Servo servo;  // create servo object to control a servo
// 16 servo objects can be created on the ESP32

int pos = 0;    // variable to store the servo position
// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

int servoPin = GPIO_NUM_4;

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

int servoPos = 90; // starting position
bool moved = false; // tracks whether servo has been moved by radio signal

void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(500);

  pinMode(LED, OUTPUT);

  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(LED, LOW);

  pinMode(BUTTON, INPUT_PULLUP);

  SPI.begin(D8, D9, D10, D0);

  if (!radio.begin()) {
    Serial.println("Radio init FAILED");
    while(1);
  }
  Serial.println("Radio init OK");

  radio.openReadingPipe(0, address);
  radio.startListening();

	servo.attach(servoPin); 

  servo.write(servoPos);

}

void loop() {
  // Move servo back 60 degrees when button is pressed, only if it was moved by radio
  if ((digitalRead(BUTTON) == LOW) && moved) {
    servoPos -= 60;
    servo.write(servoPos);
    Serial.println("Button pressed - servo returned");
    moved = false;
  }

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.print("Data received: ");
    Serial.println(text);

    servoPos += 60;
    servo.write(servoPos);

    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
    
    moved = true;
  }

  // if(digitalRead(BUTTON) == LOW) {
  //   Serial.println("Button pressed :D");
  //   delay(100);
  // }

  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
	// 	// in steps of 1 degree
	// 	myservo.write(pos/10);    // tell servo to go to position in variable 'pos'
	// 	delay(10);             // waits 15ms for the servo to reach the position
	// }
	// for (pos = 180; pos >= 180; pos -= 1) { // goes from 180 degrees to 0 degrees
	// 	myservo.write(pos);    // tell servo to go to position in variable 'pos'
	// 	delay(5);             // waits 15ms for the servo to reach the position
	// }
}