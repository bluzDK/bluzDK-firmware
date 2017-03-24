#include "application.h"

SYSTEM_MODE(MANUAL);

void setup() {
    pinMode(D7, OUTPUT);

    BLESerial.begin();
    Serial1.begin(9600);
    Serial1.println("Starting...");
}

void loop() {
    Serial1.println(BLESerial.available());
    if (BLESerial.available() != 0) {
        uint8_t bit = BLESerial.read();
        Serial1.println((char)bit);
        BLESerial.write(bit);
    }
    delay(1000);
}