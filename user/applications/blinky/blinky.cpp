#include "application.h"

bool ledOn = false;

/* executes once at startup */
void setup() {
    pinMode(D7, OUTPUT);
}

/* executes continuously after setup() runs */
void loop() {
    delay(1000);
    if (ledOn) {
        digitalWrite(D7, LOW);
    } else {
        digitalWrite(D7, HIGH);
    }
    ledOn = !ledOn;
}
