#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN 7
#define CSN_PIN 8
#define JOYSTICK_Y A0  // Joystick Y-axis

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
    int joyValue = analogRead(JOYSTICK_Y); // Read joystick Y-axis
    joyValue = map(joyValue, 0, 1023, 0, 180); // Convert joystick value to 0-180

    // Print joystick value to Serial Monitor
    Serial.print("Joystick Y: ");
    Serial.println(joyValue);

    // Send joystick value via NRF24
    bool success = radio.write(&joyValue, sizeof(joyValue));

    // Print transmission status
    if (success) {
        Serial.println("Data sent successfully!");
    } else {
        Serial.println("Failed to send data.");
    }

    delay(100); // Adjust delay as needed
}
