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
#include "rtc_timer.h"

void timerCallback()
{
  digitalWrite(D7, 1);
  if (BLE.getState() == BLE_CONNECTED && Particle.connected())
    Particle.publish("OMG IT WORKED!!");
}

RTCTimer *testTimer = new RTCTimer(2000000L, timerCallback);

/* executes once at startup */
void setup() {
  pinMode(D7, OUTPUT);  
}

/* executes continuously after setup() runs */
void loop() {
  static bool once = true;
  if (once && Particle.connected()) {
    once = false;
    Particle.publish("I'm back!");
    testTimer->start();
  }

}
