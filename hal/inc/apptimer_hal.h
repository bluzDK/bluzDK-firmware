<<<<<<< HEAD
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

#ifndef APPTIMER_HAL_H
#define	APPTIMER_HAL_H
#include <stdint.h>
#include <stddef.h> // NULL

#ifdef __cplusplus

extern void HAL_staticHandler(void *context);

extern "C" {
#endif

#include "nrf_error.h"
#include "app_error.h"

//#include "app_timer.h"
// [Grrr!!] Re-defined re-deschmined!
#ifndef APP_TIMER_H__
  typedef uint32_t app_timer_id_t;
  typedef void (*app_timer_timeout_handler_t)(void * p_context);
  typedef uint32_t (*app_timer_evt_schedule_func_t) (app_timer_timeout_handler_t timeout_handler, void *p_context);
  typedef enum {
    APP_TIMER_MODE_SINGLE_SHOT,
    APP_TIMER_MODE_REPEATED
  } app_timer_mode_t;
  extern uint32_t app_timer_create( app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
  extern uint32_t app_timer_start(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);
  extern uint32_t app_timer_stop(app_timer_id_t timer_id);
#endif
  
extern uint32_t (*HAL_app_timer_create)(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
extern uint32_t (*HAL_app_timer_start)(app_timer_id_t timer_id, uint32_t timeout_ticks, void * p_context);
extern uint32_t (*HAL_app_timer_stop)(app_timer_id_t timer_id);

#ifdef __cplusplus
} // extern "C"
#endif


#endif	/* APPTIMER_HAL_H */
=======
#ifndef APP_TIMER_HAL_H
#define APP_TIMER_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#undef STATIC_ASSERT
#include "app_timer.h"

uint32_t HAL_app_timer_create(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler);
uint32_t HAL_app_timer_start(app_timer_id_t timer_id, uint32_t milliseconds, void * p_context);
uint32_t HAL_app_timer_stop(app_timer_id_t timer_id);

#ifdef __cplusplus
}
#endif

#endif

>>>>>>> dynalib-learning

