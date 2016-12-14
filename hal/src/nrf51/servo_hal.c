/**
 ******************************************************************************
 * @file    servo_hal.c
 * @author  Matthew McGowan
 * @version V1.0.0
 * @date    27-Sept-2014
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
#include "servo_hal.h"
#include "pinmap_impl.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "app_pwm.h"
#include "nrf_gpio.h"

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/

APP_PWM_INSTANCE(PWM1,1);                   // Create the instance "PWM1" using TIMER1.
uint32_t SERVO_FREQUENCY_HZ = 50;
uint16_t last_pulse_width = 1200;

uint16_t microseconds_to_ticks(uint16_t micros)
{
    uint16_t ticks;

    double micros_per_tick = (1.0/2000000.0)*1000000.0;
    ticks = (uint16_t)(micros / micros_per_tick);

    return ticks;
}

static bool servo_enabled = false;
void servo_ready_callback(uint32_t pwm_id)    // PWM callback function
{
}

void HAL_Servo_Attach(uint16_t pin)
{
    if (servo_enabled) {
        app_pwm_disable(&PWM1);
        app_pwm_uninit(&PWM1);
    }
    nrf_gpio_cfg_output(PIN_MAP[pin].gpio_pin);
    app_pwm_config_t pwm1_cfg = APP_PWM_DEFAULT_CONFIG_1CH(1000000/SERVO_FREQUENCY_HZ, PIN_MAP[pin].gpio_pin);
    pwm1_cfg.pin_polarity[0] = APP_PWM_POLARITY_ACTIVE_HIGH;
    int err_code = app_pwm_init(&PWM1,&pwm1_cfg,servo_ready_callback);
    if (err_code != NRF_SUCCESS) {
        return;
    }
    app_pwm_enable(&PWM1);
    while (app_pwm_channel_duty_ticks_set(&PWM1, 0,  microseconds_to_ticks(last_pulse_width)) == NRF_ERROR_BUSY);
    servo_enabled = true;
}

void HAL_Servo_Detach(uint16_t pin)
{
    app_pwm_disable(&PWM1);
}

void HAL_Servo_Write_Pulse_Width(uint16_t pin, uint16_t pulseWidth)
{
    while (app_pwm_channel_duty_ticks_set(&PWM1, 0,  microseconds_to_ticks(pulseWidth)) == NRF_ERROR_BUSY);
    last_pulse_width = pulseWidth;
}

uint16_t HAL_Servo_Read_Pulse_Width(uint16_t pin)
{
    return last_pulse_width;
}

uint16_t HAL_Servo_Read_Frequency(uint16_t pin)
{
    return SERVO_FREQUENCY_HZ;
}
