#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#include <ESP32Servo.h>


#define CE_PIN D1
#define CSN_PIN D0

#define LED D5

Servo servo;  // create servo object to control a servo

// Recommended PWM GPIO pins on the ESP32 include 2,4,12-19,21-23,25-27,32-33 
// Possible PWM GPIO pins on the ESP32-S2: 0(used by on-board button),1-17,18(used by on-board LED),19-21,26,33-42
// Possible PWM GPIO pins on the ESP32-S3: 0(used by on-board button),1-21,35-45,47,48(used by on-board LED)
// Possible PWM GPIO pins on the ESP32-C3: 0(used by on-board button),1-7,8(used by on-board LED),9-10,18-21

int servoPin = GPIO_NUM_4;

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

bool latched = false; // tracks whether servo is open or closed and hence which direction to rotate in
bool moved = false; // tracks whether the receiver has already initiated servo movement

void setup() {
  delay(1000);
  Serial.begin(9600);

  pinMode(LED, OUTPUT);

  // LED flickers on and off, confirms initialization of ESP32C3 chip
  digitalWrite(LED, HIGH);
  delay(500);
  digitalWrite(LED, LOW);

  // SPI initialization for receiver using default SPI pins
  SPI.begin(D8, D9, D10, D0);

  // receiver initialization
  if (!radio.begin()) {
    Serial.println("Radio init FAILED");
    while(1);
  }
  Serial.println("Radio init OK");

  radio.openReadingPipe(0, address);
  radio.startListening();

  // Servo timing initialization
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	servo.setPeriodHertz(50);    // standard 50 hz servo
	servo.attach(servoPin, 1000, 2000);

  servo.write(60);   // Start w/ servo closed at 60 degrees

}

void loop() {

  moved = false;

  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.print("Data received: ");
    Serial.println(text);

    if(!moved && !latched) {
      // If servo in open position, adjust position back to closed and indicate servo has moved
      servo.write(94);
      Serial.println("Open");
      moved = true;
      latched = true;
    }

    if(!moved && latched) {
      // If servo in closed position, move to open and indicated servo has moved
      servo.write(60);
      digitalWrite(LED, HIGH);
      delay(200);
      digitalWrite(LED, LOW);
      Serial.println("Closed");
      moved = true;
      latched = false;
    }
  }
}