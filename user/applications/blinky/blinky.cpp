#include "application.h"

bool ledOn = false;

/* executes once at startup */
void setup() {
    pinMode(D7, OUTPUT);
}

/* executes continuously after setup() runs */
void loop() {
    if (millis() % 1000 == 0 && ledOn) {
        digitalWrite(D7, LOW);
        ledOn = !ledOn;
    } else if (millis() % 1000 == 0) {
        digitalWrite(D7, HIGH);
        ledOn = !ledOn;
    }
    System.sleep(SLEEP_MODE_DEEP);
}
