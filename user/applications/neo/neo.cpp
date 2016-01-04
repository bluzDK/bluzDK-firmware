#include "application.h"
#include "pinmap_impl.h"
#include "neopixel.h"

#define STRIP_LENGTH 12

#define PEAK_VALUE 60
#define LOW_VALUE 0
#define INCREMENT_VALUE 5

int PIXEL_PIN = PIN_MAP[D2].gpio_pin;
neopixel_strip_t m_strip;

uint8_t values[] = { 0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55 };
bool trends[] = { true, true, true, true, true, true, true, true, true, true, true, true };

void nextValues();
volatile bool update = false;
void radioCallbackHandler(bool radio_active) {
    if (radio_active == false && update) {
        neopixel_show(&m_strip);
        update = false;
    }
}

/* executes once at startup */
void setup() {
//    BLE.stopAdvertising();
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);
    BLE.registerNotifications(radioCallbackHandler);
    
    neopixel_init(&m_strip, PIXEL_PIN, STRIP_LENGTH);
    for (int i = 0; i < STRIP_LENGTH; i++)
    {
        neopixel_set_color(&m_strip, i, 0, values[i], values[i]);
    }
}

/* executes continuously after setup() runs */
void loop() {
    HAL_Delay_Milliseconds(40);
    nextValues();
    for (int i = 0; i < STRIP_LENGTH; i++) {
        neopixel_set_color(&m_strip, i, 0, 0, values[i]);
    }
    update = true;
}

void nextValues() {
    for (int i = 0; i < STRIP_LENGTH; i++) {
        if (values[i] == PEAK_VALUE) {
            trends[i] = false;
            values[i]-=INCREMENT_VALUE;
        } else if (values[i] == LOW_VALUE) {
            trends[i] = true;
            values[i]+=INCREMENT_VALUE;
        } else {
            if (trends[i]) {
                values[i]+=INCREMENT_VALUE;
            } else {
                values[i]-=INCREMENT_VALUE;
            }
        }
    }
}