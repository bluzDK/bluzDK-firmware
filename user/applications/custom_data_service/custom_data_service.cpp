#include "application.h"
SYSTEM_MODE(MANUAL);

int timeLastPublish = 0;
int timeBetweenPublishes = 300000;

void dataCallbackHandler(uint8_t *data, uint16_t length) {

}

void setup() {
    pinMode(D7, OUTPUT);
    BLE.registerDataCallback(dataCallbackHandler);

    pinMode(D6, INPUT_PULLDOWN);
    if (digitalRead(D6) == HIGH) {
        SYSTEM_MODE(AUTOMATIC);
    }
}

void loop() {
    System.sleep(SLEEP_MODE_CPU);
    if (millis() - timeLastPublish > timeBetweenPublishes)
    {
        uint8_t rsp[2];
        rsp[0] = 'H';
        rsp[1] = 'i';
        BLE.sendData(rsp, 2);
        timeLastPublish = millis();
    }
}