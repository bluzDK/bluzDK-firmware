#include "application.h"

void setup() {
    Serial1.begin(38400);
    Serial1.println("I will repeat anything you say");
}

void loop() {
    if (Serial1.available() > 0) {
        int inByte = Serial1.read();
        Serial1.println(inByte);
    }
}