#include <Servo.h>

Servo myServo;

void setup() {
  myServo.attach(2);   // Signal pin
}

void loop() {

  myServo.write(90);   // Go to 90°
  delay(1000);

  myServo.write(180);  // Go to 180°
  delay(2000);
}