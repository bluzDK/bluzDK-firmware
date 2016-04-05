/**
 ******************************************************************************
 * @file    pwm_hal.c
 * @authors Satish Nair, Brett Walach
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
#include "pwm_hal.h"
#include "pinmap_impl.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "app_pwm.h"

#define PWM_FREQUENCY_HZ 500

APP_PWM_INSTANCE(PWM1,1);                   // Create the instance "PWM1" using TIMER1.

static bool enabled = false;
static volatile bool ready_flag;            // A flag indicating PWM status.
void pwm_ready_callback(uint32_t pwm_id)    // PWM callback function
{
    ready_flag = true;
}

void HAL_PWM_Write(uint16_t pin, uint8_t value)
{
    if (enabled) {
        app_pwm_disable(&PWM1);
        app_pwm_uninit(&PWM1);
    }
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(1000000/PWM_FREQUENCY_HZ, PIN_MAP[pin].gpio_pin);
    pwm1_cfg.pin_polarity[1] = APP_PWM_POLARITY_ACTIVE_HIGH;
    int err_code = app_pwm_init(&PWM1,&pwm1_cfg,pwm_ready_callback);
    if (err_code != NRF_SUCCESS) {
        return;
    }
    app_pwm_enable(&PWM1);
    ready_flag = false;
    /* Set the duty cycle - keep trying until PWM is ready... */
    while (app_pwm_channel_duty_set(&PWM1, 0, 255-value) == NRF_ERROR_BUSY);
    enabled = true;
}

uint16_t HAL_PWM_Get_Frequency(uint16_t pin)
{
    return PWM_FREQUENCY_HZ;
}

uint16_t HAL_PWM_Get_AnalogValue(uint16_t pin)
{
    return 0;
}
