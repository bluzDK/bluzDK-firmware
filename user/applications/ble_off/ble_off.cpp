#include "application.h"


/* executes once at startup */
void setup() {
    BLE.stopAdvertising();
//    RGB.control(true);
//    RGB.color(0, 0, 0);
}

/* executes continuously after setup() runs */
void loop() {
    System.sleep(SLEEP_MODE_CPU);
}
