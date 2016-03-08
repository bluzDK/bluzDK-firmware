#include "application.h"

void dataCallbackHandler(uint8_t *data, uint16_t length) {
   digitalWrite(D7, HIGH);
}

void setup() {
    pinMode(D7, OUTPUT);
    BLE.registerDataCallback(dataCallbackHandler);
}

void loop() {
    System.sleep(SLEEP_MODE_CPU);
}