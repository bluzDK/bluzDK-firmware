#include "application.h"
SYSTEM_MODE(MANUAL);
bool sendResponse = false;
void dataCallbackHandler(uint8_t *data, uint16_t length) {
    digitalWrite(D7, HIGH);
    sendResponse = true;

}

void setup() {
    pinMode(D7, OUTPUT);
    BLE.registerDataCallback(dataCallbackHandler);
}

void loop() {
    System.sleep(SLEEP_MODE_CPU);
    if (sendResponse) {
        uint8_t rsp[2];
        rsp[0] = 0xAA;
        rsp[1] = 0xBB;
        BLE.sendData(rsp, 2);
        sendResponse = false;
    }
}