#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8
#define LEFT_JOYSTICK_X A0  
#define RIGHT_JOYSTICK_X A1
#define RIGHT_JOYSTICK_Y A2

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Communication address

// Define a struct to store joystick values
struct DataPacket {
    uint8_t LXValue; // 1 byte
    uint8_t RXValue; // 1 byte
    uint8_t RYValue; // 1 byte
} packet;

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.stopListening(); // Set as transmitter
}

void loop() {
    // Read joystick values and map to 0-255 range
    packet.LXValue = map(analogRead(LEFT_JOYSTICK_X), 0, 1023, 0, 180);
    packet.RXValue = map(analogRead(RIGHT_JOYSTICK_X), 0, 1023, 0, 180);
    packet.RYValue = map(analogRead(RIGHT_JOYSTICK_Y), 0, 1023, 0, 180); 

    // Send joystick values
    bool success = radio.write(&packet, sizeof(packet));

    // Print transmission status and values
    Serial.print("Sent: ");
    Serial.print(packet.LXValue);
    Serial.print(", ");
    Serial.print(packet.RXValue);
    Serial.print(", ");
    Serial.println(packet.RYValue);

    Serial.println(success ? "Data sent successfully!" : "Failed to send data.");

    delay(100); // Adjust delay as needed
}
