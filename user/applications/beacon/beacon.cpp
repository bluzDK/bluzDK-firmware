#include "application.h"

uint16_t major = 765;
uint16_t minor = 4321;
uint8_t UUID[16] = {0xb1, 0xe2, 0x40, 0x40, 0xb1, 0xe2, 0x40, 0x40, 0xb1, 0xe2, 0x40, 0x40, 0xb1, 0xe2, 0x40, 0x40};

void setup() {
    BLE.stopAdvertising();
//    BLE.beacon(major, minor, UUID);
    BLE.eddystone_url_beacon("http://bluz.io");
}

void loop() {

}