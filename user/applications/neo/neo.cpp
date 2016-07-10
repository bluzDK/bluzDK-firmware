#include "application.h"
#include "neopixel.h"
#include "pinmap_impl.h"

#define PEAK_VALUE 60
#define LOW_VALUE 0
#define INCREMENT_VALUE 5

//12 pixels to our ring
#define STRIP_LENGTH 12

//get the pin you wish to use
STM32_Pin_Info* pin_map = HAL_Pin_Map();
int PIXEL_PIN = pin_map[D2].gpio_pin;

//then declare a strip of neopixels
neopixel_strip_t m_strip;

//start with some initial values, we will rotate these when it runs
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
    BLE.registerNotifications(radioCallbackHandler);
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);

    neopixel_init(&m_strip, PIXEL_PIN, STRIP_LENGTH);
    for (int i = 0; i < STRIP_LENGTH; i++)
    {
        neopixel_set_color(&m_strip, i, 0, 0, values[i]);
    }
}

/* executes continuously after setup() runs */
void loop() {
    delay(40);
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