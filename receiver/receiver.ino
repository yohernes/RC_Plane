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

void setup() {
    // Initialize ESC
    esc.attach(ESC_PIN);
    
    // Send minimum throttle signal to arm the ESC
    esc.writeMicroseconds(1000);
    delay(3000);
    
    // Initialize radio
    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setPALevel(RF24_PA_MIN);  // Increased power level
    radio.startListening();
}

void loop() {
    if (radio.available()) {
        int throttle;
        radio.read(&throttle, sizeof(throttle));
        
        // Direct mapping from 0-180 to 1000-2000μs
        // Add a small deadzone at the low end (values 0-10 will be treated as 0)
        int escValue;
        if (throttle < 90) {
            escValue = 1000;  // Minimum/idle
        } else {
            escValue = map(throttle, 90, 180, 1000, 2000);
        }
        
        // Ensure values are within safe limits
        escValue = constrain(escValue, 1000, 2000);
        
        esc.writeMicroseconds(escValue);
    } else {
        // Safety feature: if no radio signal, set to idle
        static unsigned long lastSignalTime = 0;
        unsigned long currentTime = millis();
        if (currentTime - lastSignalTime > 500) {  // 500ms timeout
            esc.writeMicroseconds(1000);  // Set to minimum/idle
            lastSignalTime = currentTime;
        }
    }
}