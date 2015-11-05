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

#ifndef PINMAP_IMPL_H
#define	PINMAP_IMPL_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct STM32_Pin_Info {
  pin_t gpio_pin;
  PinMode pin_mode;
  bool adc;
} STM32_Pin_Info;

/* Exported constants --------------------------------------------------------*/

extern STM32_Pin_Info PIN_MAP[];
STM32_Pin_Info* HAL_Pin_Map(void);

extern void HAL_GPIO_Save_Pin_Mode(PinMode mode);
extern PinMode HAL_GPIO_Recall_Pin_Mode();

#define NONE ((uint8_t)0xFF)
#define ADC_CHANNEL_NONE NONE

#ifdef	__cplusplus
}
#endif

#endif	/* PINMAP_IMPL_H */

