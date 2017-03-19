/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.
 
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
  { 24, PIN_MODE_NONE, false },  /*D0*/
  { 29, PIN_MODE_NONE, false },
  { 22, PIN_MODE_NONE, false  },
  { 21, PIN_MODE_NONE, false  },
  { 23, PIN_MODE_NONE, false  },
  { 16, PIN_MODE_NONE, false  },
  { 16, PIN_MODE_NONE, false  },
  { 16, PIN_MODE_NONE, false  },
  { NONE, PIN_MODE_NONE, false  }, /*??*/
  { NONE, PIN_MODE_NONE, false  }, /*??*/
  { 16, PIN_MODE_NONE, true }, /*A0*/
  { 16, PIN_MODE_NONE, true },
  { 16, PIN_MODE_NONE, true },
  { 16, PIN_MODE_NONE, true },
  { 16, PIN_MODE_NONE, true },
  { 16, PIN_MODE_NONE, true },
  { 16, PIN_MODE_NONE, false },
  { 16, PIN_MODE_NONE, false },
  { 16, PIN_MODE_NONE, false }, /*RX*/
  { 16, PIN_MODE_NONE, false }, /*TX*/
  { 9, PIN_MODE_NONE, false }, /*BTN*/
  { 25, PIN_MODE_NONE, false }, /*RGBR*/
  { 28, PIN_MODE_NONE, false }, /*RGBG*/
  { 8, PIN_MODE_NONE, false }, /*RGBB*/
};

STM32_Pin_Info* HAL_Pin_Map() {
    return PIN_MAP;
}
