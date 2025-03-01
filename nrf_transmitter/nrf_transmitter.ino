#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);  // Create an RF24 object
const byte address[6] = "00001";  // Address for communication

void setup() {
    Serial.begin(115200);
    radio.begin();
    radio.openWritingPipe(address);  // Set the address
    radio.setPALevel(RF24_PA_LOW);   // Set power level
    radio.stopListening();           // Set as transmitter
}

void loop() {
    const char text[] = "hello";
    bool sent = radio.write(&text, sizeof(text));  // Send message

    if (sent) {
        Serial.println("Message sent: hello");
    } else {
        Serial.println("Failed to send");
    }

    delay(500);  // Send every 500ms
}
