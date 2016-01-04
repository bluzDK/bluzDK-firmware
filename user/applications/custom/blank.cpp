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
#include "sst25vf_spi.h"

uint8_t leds[18] = {D0, D1, D2, D3, D4, D5, D6, D7, A0, A1, A2, A3, A4, A5, A6, A7, RX, TX};
uint8_t rgb[3] = {21, 22, 23};
//Serial1DebugOutput debugOutput(38400);          // default is 9600 and log everything

char *hello = "Hello World";
int answer = 42;
bool sentOnce = false;
int blinkLED(String command)
{
    for (int i = 0; i < 2; i++) {
        digitalWrite(D7, HIGH);
        HAL_Delay_Milliseconds(250);
        digitalWrite(D7, LOW);
        HAL_Delay_Milliseconds(250);
    }
    if (!sentOnce) {
        Spark.publish("SendIntroTweet", "Not Done");
        sentOnce = true;
    }
    return 1;
}

bool relayOn = false;
int switchRelay(String command)
{
    if (relayOn) {
        digitalWrite(D1, LOW);
    } else {
        digitalWrite(D1, HIGH);
    }
    relayOn = !relayOn;
    return 1;
}

bool on = false;
void send() {
    if (on) {
        digitalWrite(D7, LOW);
    } else {
        digitalWrite(D7, HIGH);
    }
    on = !on;
}

/* executes once at startup */
void setup() {
//    RGB.control(true);
    DEBUG("hello world");
    pinMode(D7, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(A2, OUTPUT);

    Particle.variable("hello", hello, STRING);
    Particle.variable("answer", &answer, INT);
    Particle.function("blink", blinkLED);
    Particle.function("relay", switchRelay);
    
//    pinMode(D2, INPUT_PULLUP);
//    attachInterrupt(D2, send, FALLING);
//    
//    noInterrupts();
//    interrupts();
    
//    SPI.begin();
//    SPI.setBitOrder(LSBFIRST);
//    SPI.setClockDivider(SPI_CLOCK_DIV128);
//    SPI.setDataMode(SPI_MODE0);
    
//    BLE.stopAdvertising();
    
//    int spi_err = sFLASH_SelfTest();
//    if (spi_err != 0) {
//        while(1) {
//            digitalWrite(D7, HIGH);
//            HAL_Delay_Milliseconds(400);
//            digitalWrite(D7, LOW);
//            HAL_Delay_Milliseconds(400);
//        }
//    }
//    
//    
//    //copy device int to external flash
//    uint8_t buf[2];
//    memcpy(buf, (const void *)0x3F000, 2);
//    sFLASH_EraseSector(FLASH_DEVICE_INT);
//    sFLASH_WriteBuffer(buf, FLASH_DEVICE_INT, 2);
    
    //For Snake
//    for (int i = 0; i < 18; i++) {
//        pinMode(leds[i], OUTPUT);
//    }
//    for (int i = 0; i < 3; i++) {
//        pinMode(rgb[i], OUTPUT);
//    }
}

/* executes continuously after setup() runs */
void loop() {
//    digitalWrite(D7, HIGH);
//    HAL_Delay_Milliseconds(500);
//    digitalWrite(D7, LOW);
//    HAL_Delay_Milliseconds(500);
//    HAL_Delay_Milliseconds(1000);
//    DEBUG("System milliseconds: ");
//    DEBUG("%d", millis());
    
//    if (millis() == 10000) {
//        BLE.startAdvertising();
//    } else if (millis() == 20000) {
//        BLE.stopAdvertising();
//    }
    
    System.sleep(SLEEP_MODE_DEEP);
//    if (millis() % 100 == 0) {
////        DEBUG("SPI Send");
//        digitalWrite(A2, LOW);
//        SPI.transfer(0x55);
//        digitalWrite(A2, HIGH);
//    }
    //Snake
//    for (int i = 0; i < 18; i++) {
//        digitalWrite(leds[i], HIGH);
//        if (i > 0) {
//            digitalWrite(leds[i-1], LOW);
//        }
//        HAL_Delay_Milliseconds(150);
//    }
//    digitalWrite(leds[17], LOW);
//    RGB.color(255, 0, 0);
//    HAL_Delay_Milliseconds(250);
//    RGB.color(0, 255, 0);
//    HAL_Delay_Milliseconds(250);
//    RGB.color(0, 0, 255);
//    HAL_Delay_Milliseconds(250);
//    RGB.color(0, 0, 0);
}
