/**
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.
  Copyright (c) 2016 Bryan J. Rentoul (aka Gruvin).  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

#ifndef __RTC_TIMER_H_
#define	__RTC_TIMER_H_
#include <stdint.h>
#include <stddef.h> // NULL

#ifdef __cplusplus
extern "C" {
#endif

#include "nrf_error.h"
#include "app_error.h"
#include "apptimer_hal.h"

#ifdef __cplusplus
}
#endif

class RTCTimer
{
public:
    RTCTimer(uint32_t interval, app_timer_timeout_handler_t callback);
    RTCTimer(uint32_t interval, app_timer_timeout_handler_t callback, bool oneshot);
    void start();
    void stop();
    void changePeriod();
    void dispose();
    bool isActive();
    uint32_t getError();

private:
    void _init();
    app_timer_id_t timerID;
    uint32_t timerInterval;
    app_timer_timeout_handler_t callbackFunc;
    bool oneShot;
    void *callbackContextPointer;
};



#endif	/* __RTC_TIMER_H_ */

