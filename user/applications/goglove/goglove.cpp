#include "application.h"

#define ONE D0
#define TWO D1
#define THREE D2
#define FOUR D3
#define FIVE D4

#define BUTTON_COUNT 5

#define DEBOUNCE_TIMEOUT 250

bool send_event = false;
long debounce = 0;
long time_since_last_activity = 0;
int last_button_index = 0;

typedef struct button_commands {
    int button;
    String command;

} button_commands;

button_commands buttons[BUTTON_COUNT]={
        {ONE, "ONE"},
        {TWO, "TWO"},
        {THREE, "THREE" },
        {FOUR, "FOUR" },
        {FIVE, "FIVE" }
};


void setup() {
    BLE.setConnectionParameters(20,40);
    // setup each button to receive on interrupt
    for (int i = 0; i < BUTTON_COUNT; i++) {
        pinMode(buttons[i].button, INPUT_PULLUP);
    }
}

void loop() {
    System.sleep(SLEEP_MODE_CPU);

    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (digitalRead(buttons[i].button) == LOW && millis() - debounce >  DEBOUNCE_TIMEOUT) {
            send_event = true;
            debounce = millis();
            time_since_last_activity = millis();
            Particle.publish("GoGlove Remote", buttons[i].command, PRIVATE);
            uint8_t data[1] = {i+1};
            BLE.sendData(data, sizeof(data));
        }
    }
}