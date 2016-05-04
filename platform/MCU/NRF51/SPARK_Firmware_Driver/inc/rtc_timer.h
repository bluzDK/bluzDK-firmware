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

//#include "app_timer.h"
// [Grrr!!]
//    app_timer.h is not re-entrant in mixed C and C++ terms, due to their of checks 
//    for macro definitions (I think?) So, rather than changing Nordic's code and 
//    hopefully only for now, I've copied the required bits here :-/  ...
// [/Grrr!!]

  typedef uint32_t app_timer_id_t;
  typedef void (*app_timer_timeout_handler_t)(void * p_context);
  typedef uint32_t (*app_timer_evt_schedule_func_t) (app_timer_timeout_handler_t timeout_handler, void *p_context);
  typedef enum
  {
    APP_TIMER_MODE_SINGLE_SHOT,                 /**< The timer will expire only once. */
    APP_TIMER_MODE_REPEATED                     /**< The timer will restart each time it expires. */
  } app_timer_mode_t;

  extern uint32_t app_timer_create(
      app_timer_id_t *            p_timer_id,
      app_timer_mode_t            mode,
      app_timer_timeout_handler_t timeout_handler);
  extern uint32_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);
  extern uint32_t app_timer_stop(app_timer_id_t timer_id);
// [/Grrr!!]

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
    uint32_t errorCode;

};

#endif	/* __RTC_TIMER_H_ */

