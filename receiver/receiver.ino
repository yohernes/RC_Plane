#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define CE_PIN 7
#define CSN_PIN 8
#define ESC_PIN 9  // ESC signal pin
#define SERVO1_PIN 5 // First servo (e.g., elevator)
#define SERVO2_PIN 6 // Second servo (reversed ailerons, connected together)

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001"; // Same address as transmitter
Servo esc;  // ESC controlled as a servo
Servo servo1, servo2;  // Additional servos

unsigned long lastMessageTime = 0;  // Timestamp of last received message
const unsigned long messageTimeout = 3000;  // Timeout in milliseconds (3 seconds)

// Define struct to match transmitter's packet
struct DataPacket {
    uint8_t LXValue; // 1 byte (left joystick X - throttle)
    uint8_t RXValue; // 1 byte (right joystick X - elevator)
    uint8_t RYValue; // 1 byte (right joystick Y - reversed ailerons)
} packet;

void setup() {
    // Attach ESC and servos
    esc.attach(ESC_PIN);
    servo1.attach(SERVO1_PIN);
    servo2.attach(SERVO2_PIN);
    
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
        radio.read(&packet, sizeof(packet));
        
        // Update the last message timestamp
        lastMessageTime = millis();
        
        // Convert throttle (LXValue) to ESC range (1000-2000μs)
        int escValue;
        if (packet.LXValue < 10) {
            escValue = 1000;  // Minimum/idle
        } else {
            escValue = map(packet.LXValue, 10, 180, 1000, 2000);
        }
        esc.writeMicroseconds(escValue);
        
        // Convert right joystick values to servo range (0-180 degrees)
        int servo1Angle = map(packet.RXValue, 0, 180, 0, 180); // Elevator
        int servo2Angle = map(packet.RYValue, 0, 180, 180, 0); // Reversed ailerons
        
        // Set servo positions
        servo1.write(servo1Angle);
        servo2.write(servo2Angle);
    }
    
    // Check if we've exceeded the timeout period
    if (millis() - lastMessageTime > messageTimeout) {
        // No message received for 3 seconds, stop the motor and reset servos
        esc.writeMicroseconds(1000);  // Set to minimum throttle
        servo1.write(90);  // Neutral position
        servo2.write(90);  // Neutral position
    }
}
