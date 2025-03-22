#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE_PIN 7
#define CSN_PIN 8
#define ESC_PIN 9  // ESC signal pin

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Same address as transmitter
Servo esc;  // ESC controlled as a servo

unsigned long lastMessageTime = 0;  // Timestamp of last received message
const unsigned long messageTimeout = 3000;  // Timeout in milliseconds (3 seconds)

void setup() {
    // Initialize ESC
    esc.attach(ESC_PIN);
    
    // Send minimum throttle signal to arm the ESC
    esc.writeMicroseconds(1000);
    delay(3000);
    
    // Initialize radio
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_HIGH);  // Increased power level
    radio.startListening();
    
    // Initialize the last message time to current time
    lastMessageTime = millis();
}

void loop() {
    // Check if a signal is available
    if (radio.available()) {
        int throttle;
        radio.read(&throttle, sizeof(throttle));
        
        // Update the last message timestamp
        lastMessageTime = millis();
        
        // Direct mapping from 0-180 to 1000-2000μs
        // Add a small deadzone at the low end (values 0-10 will be treated as 0)
        int escValue;
        if (throttle < 10) {
            escValue = 1000;  // Minimum/idle
        } else {
            escValue = map(throttle, 10, 180, 1000, 2000);
        }
        
        // Ensure values are within safe limits
        escValue = constrain(escValue, 1000, 2000);
        
        esc.writeMicroseconds(escValue);
    }
    
    // Check if we've exceeded the timeout period
    if (millis() - lastMessageTime > messageTimeout) {
        // No message received for 3 seconds, stop the motor
        esc.writeMicroseconds(1000);  // Set to minimum throttle
    }
}