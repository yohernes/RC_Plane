#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

RF24 radio(7, 8); // CE, CSN
Servo myServo;
const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  myServo.attach(9);
  Serial.println("Receiver Ready");
}

void loop() {
  if (radio.available()) {
    int receivedValue;
    radio.read(&receivedValue, sizeof(receivedValue));

    Serial.print("Received: ");
    Serial.println(receivedValue);

    myServo.write(receivedValue);
  } else {
    Serial.println("Waiting for data...");
  }

  delay(1000);
}
