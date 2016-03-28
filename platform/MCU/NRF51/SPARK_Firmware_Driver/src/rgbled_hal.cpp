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

#include "rgbled_hal.h"
#include "gpio_hal.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "hw_layout.h"
#include "nrf_gpio.h"

uint16_t rgbValues[3];
void Set_RGB_LED_Values(uint16_t r, uint16_t g, uint16_t b)
{
    if (r > 0x00) {
        nrf_gpio_pin_clear(RGB_LED_PIN_RED);
    } else {
        nrf_gpio_pin_set(RGB_LED_PIN_RED);
    }
    
    if (g > 0x00) {
        nrf_gpio_pin_clear(RGB_LED_PIN_GREEN);
    } else {
        nrf_gpio_pin_set(RGB_LED_PIN_GREEN);
    }
    
    if (b > 0x00) {
        nrf_gpio_pin_clear(RGB_LED_PIN_BLUE);
    } else {
        nrf_gpio_pin_set(RGB_LED_PIN_BLUE);
    }
    
#if PLATFORM_ID==269
    if (r > 0x00 || g > 0x00 || b > 0x00) {
         nrf_gpio_pin_set(0);
    } else {
        nrf_gpio_pin_clear(0);
    }
#endif
    
    rgbValues[0] = r;
    rgbValues[1] = g;
    rgbValues[2] = b;
}

void Get_RGB_LED_Values(uint16_t* rgb)
{
    rgb[0] = rgbValues[0];
    rgb[1] = rgbValues[1];
    rgb[2] = rgbValues[2];
}

void Set_User_LED(uint8_t state)
{

}

void Toggle_User_LED()
{

}

uint16_t Get_RGB_LED_Max_Value()
{
    return 65535;
}
