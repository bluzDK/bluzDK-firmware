/**
 ******************************************************************************
 * @file    hal_dynalib_apptimer.h
 * @authors Matthew McGowan, Bryan J. Rentoul (aka Gruvin)
 * @date    04 May 2016
 ******************************************************************************
  Copyright (c) 2015 Particle Industries, Inc.  All rights reserved.

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

#ifndef HAL_DYNALIB_APPTIMER_H
#define HAL_DYNALIB_APPTIMER_H

#include "dynalib.h"

#ifdef DYNALIB_EXPORT
#include "apptimer_hal.h"
#endif

// WARNING
// The order of functions must not be changed or older applications will break
// when used with newer system firmware.
// Function signatures shouldn't be changed other than changing pointer types.
// New HAL functions must be added to the end of this list.
// GNINRAW

DYNALIB_BEGIN(hal_apptimer)
DYNALIB_FN(0, hal_apptimer, HAL_app_timer_create, uint32_t(app_timer_id_t *p_timer_id, app_timer_mode_t mode, app_timer_timeout_handler_t timeout_handler))
DYNALIB_FN(1, hal_apptimer, HAL_app_timer_start, uint32_t(app_timer_id_t timer_id, uint32_t milliseconds, void *p_context))
DYNALIB_FN(2, hal_apptimer, HAL_app_timer_stop, uint32_t(app_timer_id_t timer_id))
DYNALIB_END(hal_apptimer)

#endif	/* HAL_DYNALIB_APPTIMER_H */

