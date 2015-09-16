/**
 ******************************************************************************
 * @file    pinmap_hal.c
 * @authors Satish Nair, Brett Walach, Matthew McGowan
 * @version V1.0.0
 * @date    12-Sept-2014
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

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

/* Includes ------------------------------------------------------------------*/
#include "pinmap_hal.h"
#include "pinmap_impl.h"
#include <stddef.h>

/* Private typedef -----------------------------------------------------------*/

STM32_Pin_Info PIN_MAP[TOTAL_PINS] =
{
/*
 * gpio_pin (0-31)
 * pin_mode (NONE by default, can be set to OUTPUT, INPUT, or other types)
 * adc (true if supports adc, false otherwise)
 */
  { 21, PIN_MODE_NONE, false },  /*D0*/
  { 22, PIN_MODE_NONE, false },
  { 23, PIN_MODE_NONE, false  },
  { 24, PIN_MODE_NONE, false  },
  { 25, PIN_MODE_NONE, false  },
  { 28, PIN_MODE_NONE, false  },
  { 29, PIN_MODE_NONE, false  },
  { 30, PIN_MODE_NONE, false  },
  { NONE, PIN_MODE_NONE, false  }, /*??*/
  { NONE, PIN_MODE_NONE, false  }, /*??*/
  { 6, PIN_MODE_NONE, true }, /*A0*/
  { 5, PIN_MODE_NONE, true },
  { 4, PIN_MODE_NONE, true },
  { 3, PIN_MODE_NONE, true },
  { 2, PIN_MODE_NONE, true },
  { 1, PIN_MODE_NONE, true },
  { 0, PIN_MODE_NONE, false },
  { 13, PIN_MODE_NONE, false },
  { 12, PIN_MODE_NONE, false }, /*RX*/
  { 8, PIN_MODE_NONE, false }, /*TX*/
  { 7, PIN_MODE_NONE, false } /*BTN*/
};

STM32_Pin_Info* HAL_Pin_Map() {
    return PIN_MAP;
}
