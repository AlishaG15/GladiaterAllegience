#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN D1
#define CSN_PIN D0

#define LED D5

#define BUTTON D3

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

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

}

void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));

    Serial.print("Data received: ");
    Serial.println(text);

    digitalWrite(LED, HIGH);
    delay(200);
    digitalWrite(LED, LOW);
  }

  if(digitalRead(BUTTON) == LOW) {
    Serial.println("Button pressed :D");
    delay(100);
  }
}