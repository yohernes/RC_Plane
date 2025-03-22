#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8
#define LEFT_JOYSTICK_X A0  // Joystick Y-axis
#define RIGHT_JOYSTICK_X A1
#define RIGHT_JOYSTICK_Y A2


RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Address for communication

void setup() {
    Serial.begin(9600);
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MAX);
    radio.stopListening(); // Set as transmitter
}

void loop() {
    int LXValue = analogRead(LEFT_JOYSTICK_X); // Read joystick Y-axis
    LXValue = map(LXValue, 0, 1023, 0, 180); // Convert joystick value to 0-180
    
    int RXValue = analogRead(RIGHT_JOYSTICK_X);
    RXValue = map(RXValue, 0, 1023, 0, 180);

    int RYValue = analogRead(RIGHT_JOYSTICK_Y);
    RYValue = map(RXValue, 0, 1023, 0, 180);


    char packet[50];
    // Print joystick values to Serial Monitor
    snprintf(packet, sizeof(packet), "%d,%d,%d", LXValue, RXValue, RYValue);

    Serial.println(packet);

    // Send joystick value via NRF24
    bool success = radio.write(&packet, sizeof(packet));

    // Print transmission status
    if (success) {
        Serial.println("Data sent successfully!");
    } else {
        Serial.println("Failed to send data.");
    }

    delay(100); // Adjust delay as needed
}
