/**
 ******************************************************************************
 * @file    timer.cpp
 * @authors Bryan J. Rentoul (aka Gruvin), Satish Nair, Zachary Crockett, Mohit Bhoite and Brett Walach
 * @version V0.1
 * @date    8-May-2016
 * @brief   Timer class; specifically for BluzDK SoftDevice, low power RTC Application Timers
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.
  Copyright (c) 2013-2016 Bryan J. Rentoul (aka Gruvin). All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "application.h"

/* Function prototypes -------------------------------------------------------*/
void timer1_Callback();

class TimerTest : public Timer
{
  uint16_t counter;

  public:
  TimerTest(uint32_t interval) 
    : Timer(interval, nullptr) // no callback, as we'll be simply overriding Timer::timeout 
  {
  };
  
  virtual void timeout()
  {
    if (Particle.connected())
    {
      if (millis() < 60000) 
        Particle.publish("Yiiiup! Me too! :-)", String(millis()));
      else
      {
        stop(); 
        Particle.publish("Yiiiup! I'm otta here!", String(millis()));
      }
    }
  };
};

TimerTest classMethodOverrider(20000);

///////////////////////////////////////////////////////////
// standard function as the timeout handler callback ...
Timer timer1(6300, timer1_Callback, /*one_shot=*/true ); 

void timer1_Callback()
{
  if (Particle.connected())
  {
    Particle.publish("IT WORKS!!! :-D C'ya!", String(millis()));
  } 
}


void setup()
{
    classMethodOverrider.start();
}

void loop()
{
  static bool once = true;
  if (once && Particle.connected()) {
    once = false;
    Particle.publish("I'm back!", String(millis()));
    timer1.start();
  }

}

