#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";  // Same address as transmitter

void setup() {
    Serial.begin(9600);
    radio.begin();
    delay(100);
    Serial.println("setting up...");
    radio.openReadingPipe(0, address);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_HIGH);
    radio.startListening(); // Set as receiver
    
}

void loop() {
    if (radio.available()) {
        char text[32] = "";
        Serial.println("waiting...");
        radio.read(&text, 5);
        Serial.print("Received: ");
        Serial.println(text);
    }
}
