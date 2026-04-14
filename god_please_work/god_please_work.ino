#include <Arduino.h>

#define led D5

void setup() {
  delay(1000);
  Serial.begin(9600);
  delay(100);
  Serial.println("Starting...");
  pinMode(led, OUTPUT);
}

void loop() {
  Serial.println("Loop running");
  digitalWrite(led, HIGH);
  delay(1000);
  digitalWrite(led, LOW);
  delay(1000);
}