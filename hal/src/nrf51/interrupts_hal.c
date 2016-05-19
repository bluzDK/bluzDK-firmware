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
#include "interrupts_hal.h"
#include "pinmap_impl.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "nrf.h"
#include "app_error.h"
#include "nrf_gpio.h"
#include "nrf_drv_gpiote.h"
#include "debug.h"

#define MAX_PIN_COUNT       19
#define PIN_ACTIVE_TOGGLE   2                               /**< Indicates that an interrupt is toggle. */
#define PIN_ACTIVE_HIGH     1                               /**< Indicates that an interrupt is active high. */
#define PIN_ACTIVE_LOW      0                               /**< Indicates that an interrupt is active low. */

typedef void (*pin_interrupt_handler_t)(void* data);

/**@brief Button configuration structure. */
typedef struct
{
    uint8_t                 pin_no;           /**< Pin to be used as an interrupt. */
    uint8_t                 pull;             /**< State of the pull up/down resistor. */
    uint8_t                 active_state;     /**< PIN_ACTIVE_HIGH or PIN_ACTIVE_LOW. */
    pin_interrupt_handler_t pin_handler;      /**< Handler to be called when interrupt is triggered. */
    void*                   data;             /**< Data? */
    bool                    is_enabled;       /**< Indicates if the pin interrupt is enabled. */
} pin_cfg_t;

static pin_cfg_t interrupts[MAX_PIN_COUNT];

static void gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    for (int i = 0; i < MAX_PIN_COUNT; i++)
    {
        if (interrupts[i].pin_no == pin) {
            bool trigger = false;
            switch (action){
                case NRF_GPIOTE_POLARITY_LOTOHI:
                    if (interrupts[i].active_state == PIN_ACTIVE_HIGH || interrupts[i].active_state == PIN_ACTIVE_TOGGLE) {
                        trigger = true;
                    }
                    break;
                case NRF_GPIOTE_POLARITY_HITOLO:
                    if (interrupts[i].active_state == PIN_ACTIVE_LOW || interrupts[i].active_state == PIN_ACTIVE_TOGGLE) {
                        trigger = true;
                    }
                    break;
                case NRF_GPIOTE_POLARITY_TOGGLE:
                    trigger = true;
                    break;
            }
            if (trigger && interrupts[i].pin_handler != NULL) {
                interrupts[i].pin_handler(interrupts[i].data);
            }
        }
    }
}

void HAL_Interrupts_Attach(uint16_t pin, HAL_InterruptHandler handler, void* data, InterruptMode mode, HAL_InterruptExtraConfiguration* extraConfig)
{
    if (!nrf_drv_gpiote_is_init())
    {
        int err_code = nrf_drv_gpiote_init();
        if (err_code != NRF_SUCCESS)
        {
                 return;
        }
    }
    
    interrupts[pin].pin_no = PIN_MAP[pin].gpio_pin;
    interrupts[pin].pin_handler = handler;
    interrupts[pin].data = data;
    
    nrf_drv_gpiote_in_config_t config;
    config.is_watcher = false;
    config.hi_accuracy = false;
    config.pull = NRF_GPIO_PIN_NOPULL;
    
    switch (mode) {
        case CHANGE:
            config.sense = NRF_GPIOTE_POLARITY_TOGGLE;
            interrupts[pin].active_state = PIN_ACTIVE_TOGGLE;
            break;
        case RISING:
            config.sense = NRF_GPIOTE_POLARITY_LOTOHI;
            interrupts[pin].active_state = PIN_ACTIVE_HIGH;
            break;
        case FALLING:
            config.sense = NRF_GPIOTE_POLARITY_HITOLO;
            interrupts[pin].active_state = PIN_ACTIVE_LOW;
            break;
    }
    switch (PIN_MAP[pin].pin_mode) {
        case INPUT_PULLDOWN:
            config.pull = NRF_GPIO_PIN_PULLDOWN;
            break;
        case INPUT_PULLUP:
            config.pull = NRF_GPIO_PIN_PULLUP;
            break;
        case INPUT:
            config.pull = NRF_GPIO_PIN_NOPULL;
            break;
    }
    interrupts[pin].pull = config.pull;
    
    int err_code = nrf_drv_gpiote_in_init(PIN_MAP[pin].gpio_pin, &config, gpiote_event_handler);
    if (err_code != NRF_SUCCESS)
    {
        return;
    }
    nrf_drv_gpiote_in_event_enable(PIN_MAP[pin].gpio_pin, true);
    interrupts[pin].is_enabled = true;
}

void HAL_Interrupts_Detach(uint16_t pin)
{
    nrf_drv_gpiote_in_event_disable(interrupts[pin].pin_no);
    interrupts[pin].pin_handler = NULL;
    interrupts[pin].is_enabled = false;
}

void HAL_Interrupts_Enable_All(void)
{
    for (int i = 0; i < MAX_PIN_COUNT; i++) {
        if (interrupts[i].is_enabled == false && interrupts[i].pin_handler != NULL) {
            DEBUG("enabling %d", interrupts[i].pin_no);
            nrf_gpio_cfg_input(interrupts[i].pin_no, interrupts[i].pull);
            nrf_drv_gpiote_in_event_enable(interrupts[i].pin_no, true);
            interrupts[i].is_enabled = true;
        }
    }
}

void HAL_Interrupts_Disable_All(void)
{
    for (int i = 0; i < MAX_PIN_COUNT; i++) {
        if (interrupts[i].is_enabled == true && interrupts[i].pin_handler != NULL) {
            DEBUG("disabling %d", interrupts[i].pin_no);
            nrf_drv_gpiote_in_event_disable(interrupts[i].pin_no);
            interrupts[i].is_enabled = false;
        }
    }
}

void HAL_Interrupts_Suspend(void)
{
}

void HAL_Interrupts_Restore(void)
{
}

/*******************************************************************************
 * Function Name  : HAL_EXTI_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function is called by any of the interrupt handlers. It
                                                 essentially fetches the user function pointer from the array
                                                 and calls it.
 * Input          : EXTI_Line (Supported: 0, 1, 3, 4, 5, 6, 7, 13, 14 and 15)
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_EXTI_Handler(uint8_t EXTI_Line)
{
}


int HAL_disable_irq()
{
    return 0;
}

void HAL_enable_irq(int is)
{
}

