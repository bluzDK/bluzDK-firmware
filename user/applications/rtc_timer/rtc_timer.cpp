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

void timerCallback()
{
// The following line shoud not strictly be needed. This should be fixed pretty soon though.
  if (!Particle.connected()) return;

  digitalWrite(D7, 1);
  Particle.publish("Timer (class of 2016)", String(millis()) );
  delay(200);
  digitalWrite(D7, 0);
}

Timer testTimer(5000, timerCallback);

/* executes once at startup */
void setup() {
  pinMode(D7, OUTPUT);  

// The following doesn't work as it this writing. It should be fixed pretty soon though.
//  Particle.publish("I'm back!", String(millis()) );
//  testTimer.start();

}

/* executes continuously after setup() runs */
void loop() {
  static bool one_shot = true;
  if (one_shot && Particle.connected()) {
    one_shot = false;

    Particle.publish("I'm back!", String(millis()) );
    testTimer.start();
  }

}
