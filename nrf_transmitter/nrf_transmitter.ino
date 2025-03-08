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
  Serial.println("Transmitter Ready");
}

void loop() {
  int joystickY = analogRead(A1);
  int mappedValue = map(joystickY, 0, 1023, 0, 180);

  Serial.print("Sending: ");
  Serial.println(mappedValue);

  bool success = radio.write(&mappedValue, sizeof(mappedValue));

  if (success) {
    Serial.println("✅ Sent successfully");
  } else {
    Serial.println("❌ Send failed!");
  }

  delay(1000);
}
