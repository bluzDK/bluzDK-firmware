/*
 ******************************************************************************
 *  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

 #include "application.h"

uint8_t leds[18] = {D0, D1, D2, D3, D4, D5, D6, D7, A0, A1, A2, A3, A4, A5, A6, A7, RX, TX};
Serial1DebugOutput debugOutput(38400);          // default is 9600 and log everything

int answer = 42;
bool sentOnce = false;
int blinkLED(String command)
{
    for (int i = 0; i < 3; i++) {
        digitalWrite(D7, HIGH);
        HAL_Delay_Milliseconds(500);
        digitalWrite(D7, LOW);
        HAL_Delay_Milliseconds(500);
    }
    if (!sentOnce) {
        Spark.publish("SendIntroTweet", "Not Done");
        sentOnce = true;
    }
    return 1;
}

/* executes once at startup */
void setup() {
    DEBUG("hello world");
    pinMode(D7, OUTPUT);

    Spark.variable("answer", &answer, INT);
    Spark.function("blink", blinkLED);
    
    
    //For Snake
//    for (int i = 0; i < 18; i++) {
//        pinMode(leds[i], OUTPUT);
//    }
}

/* executes continuously after setup() runs */
void loop() {
    
//    HAL_Delay_Milliseconds(1000);
//    DEBUG("System milliseconds: ");
//    DEBUG("%d", millis());
    
    System.sleep(SLEEP_MODE_DEEP);
    //Snake
//    for (int i = 0; i < 18; i++) {
//        digitalWrite(leds[i], HIGH);
//        if (i > 0) {
//            digitalWrite(leds[i-1], LOW);
//        }
//        HAL_Delay_Milliseconds(100);
//    }
//    digitalWrite(leds[17], LOW);
}
