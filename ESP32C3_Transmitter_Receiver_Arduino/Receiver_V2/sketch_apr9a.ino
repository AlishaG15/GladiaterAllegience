#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN GPIO_NUM_2
#define CSN_PIN GPIO_NUM_3

RF24 radio(CE_PIN, CSN_PIN);

const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);
  while (!Serial);

  Serial.println("Starting Receiver...");

  if (!radio.begin()) {
    Serial.println("Radio hardware NOT responding!");
    while (1);
  }

  Serial.println("Radio initialized OK");

  radio.setPALevel(RF24_PA_LOW);   // Prevent power instability
  radio.setDataRate(RF24_1MBPS);

  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("Listening...");
}

void loop()
{
  digitalWrite(2, LOW);
  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    
    Serial.print("Data received: ");
    Serial.println(text);

    if (strcmp(text, "Hello World") == 0) {
      digitalWrite(2, HIGH);
      delay(300);
    }

  }
}