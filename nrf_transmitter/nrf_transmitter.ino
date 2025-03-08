#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  Serial.println("starting...");
  if (radio.available())
  {
      Serial.println("radio available");

  }
}

void loop() {
  const char text[] = "Hello world";
  if(radio.write(&text, sizeof(text)))
  {
      Serial.println("sending");
  };
  delay(1000);
}