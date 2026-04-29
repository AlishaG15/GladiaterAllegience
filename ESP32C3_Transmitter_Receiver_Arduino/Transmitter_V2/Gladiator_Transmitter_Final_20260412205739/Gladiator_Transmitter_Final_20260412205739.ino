//Include Libraries
#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define pins
#define CE_PIN D1
#define CSN_PIN D0

#define led D4

#define button D2

// Create RF24 radio object
RF24 radio(CE_PIN, CSN_PIN);

//address through which two modules communicate.
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  delay(1000);

  SPI.begin(D8, D9, D10, D0); // SCK, MISO, MOSI, SS
  radio.begin();

  pinMode(led, OUTPUT);
  pinMode(button, INPUT);

  Serial.begin(9600);
  delay(100);

  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH); // immediate LED test
  delay(1000);
  digitalWrite(led, LOW);

  if (!radio.begin()) {
    Serial.println("Radio init FAILED");
    while (1); // halt so you can see the failure
  }
  Serial.println("Radio init OK");

  //set the address
  radio.openWritingPipe(address);

  //Set module as transmitter
  radio.stopListening();
}
void loop() {
  if(digitalRead(button) == LOW) {
    //Send message to receiver
    const char text[] = "Hello World";
    radio.write(&text, sizeof(text));

    Serial.print("Data sent: ");
    Serial.println(text);

    // Replace lines 51 - 54 w/ the following two lines to check if the receiver is acknowledging receipt of the text. 
    // bool success = radio.write(&text, sizeof(text));
    // Serial.println(success ? "TX OK" : "TX FAILED");  

    digitalWrite(led, HIGH);

    delay(1000);

    digitalWrite(led, LOW);
  }
}