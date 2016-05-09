/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.
 Copyright (c) 2016 Bryan J. Rentoul (aka Gruvin).  All rights reserved.
 
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
 */

/* Includes -----------------------------------------------------------------*/
#include "rtc_timer.h"

/* Private typedef ----------------------------------------------------------*/

/* Private define -----------------------------------------------------------*/
#define MAX_RTC_TIMERS (APP_TIMER_MAX_TIMERS-1)

/* Private macro ------------------------------------------------------------*/

/* Private variables --------------------------------------------------------*/

/* Extern variables ---------------------------------------------------------*/

/* Private function prototypes ----------------------------------------------*/

RTCTimer::RTCTimer(uint32_t interval, void (*handler_fn)(void))
{
  _init();
  handlerFunc = handler_fn;
  timerInterval = interval;
  timerMode = APP_TIMER_MODE_SINGLE_SHOT;
};

RTCTimer::RTCTimer(uint32_t interval, void (*handler_fn)(void), bool one_shot = true)
{
  _init();
  handlerFunc = handler_fn;
  timerInterval = interval;
  timerMode = (one_shot) ? APP_TIMER_MODE_SINGLE_SHOT : APP_TIMER_MODE_REPEATED;
};

RTCTimer::~RTCTimer()
{
  if (timerID) app_timer_stop(timerID);
}

void RTCTimer::_init() 
{ 
  handlerFunc = NULL; 
  handlerContext = NULL;
  timerMode = APP_TIMER_MODE_REPEATED;
  timerID = 0;
  active = false;
};

void RTCTimer::start()
{
  uint32_t err_code;
  if (!timerID)  // don't repeat this call if we already have a valid timerID
  {
    err_code = HAL_app_timer_create( &timerID, timerMode, HAL_staticHandler );
    APP_ERROR_CHECK(err_code);    
  }

  if (timerID) 
  {
    err_code = HAL_app_timer_start(timerID, timerInterval, this);
    APP_ERROR_CHECK(err_code);    
  }
};

void RTCTimer::stop()
{
  app_timer_stop(timerID);
};

void RTCTimer::changePeriod()
{
};

void RTCTimer::dispose()
{
};

bool RTCTimer::isActive()
{
  return active;
};

void RTCTimer::timeout_handler()
{
  if (handlerFunc) { handlerFunc(); }
};

void HAL_staticHandler(void *context)
{
    if (!context) return;
    RTCTimer *instance = static_cast<RTCTimer *>(context);
    instance->active = true;
    instance->timeout_handler();
    instance->active = false;
}
