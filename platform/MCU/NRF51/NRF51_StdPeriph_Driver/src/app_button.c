/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "app_button.h"
#include <string.h>
#include "nordic_common.h"
#include "app_util.h"
#include "app_timer.h"
#include "app_error.h"
#include "nrf_drv_gpiote.h"
#include "nrf_assert.h"

static app_button_cfg_t *             mp_buttons = NULL;           /**< Button configuration. */
static uint8_t                        m_button_count;              /**< Number of configured buttons. */
static uint32_t                       m_detection_delay;           /**< Delay before a button is reported as pushed. */
static app_timer_id_t                 m_detection_delay_timer_id;  /**< Polling timer id. */


static uint32_t m_pin_state;
static uint32_t m_pin_transition;

/**@brief Function for handling the timeout that delays reporting buttons as pushed.
 *
 * @details    The detection_delay_timeout_handler(...) is a call-back issued from the app_timer
 *             module. It is called with the p_context parameter. The p_context parameter is
 *             provided to the app_timer module when a timer is started, using the call
 *             @ref app_timer_start. On @ref app_timer_start the p_context will be holding the
 *             currently pressed buttons.
 *
 * @param[in]  p_context   Pointer used for passing information app_start_timer() was called.
 *                         In the app_button module the p_context holds information on pressed
 *                         buttons.
 */
static void detection_delay_timeout_handler(void * p_context)
{
    uint8_t i;
    
    // Pushed button(s) detected, execute button handler(s).
    for (i = 0; i < m_button_count; i++)
    {
        app_button_cfg_t * p_btn = &mp_buttons[i];
        uint32_t btn_mask = 1 << p_btn->pin_no;
        if (btn_mask & m_pin_transition)
        {
            m_pin_transition &= ~btn_mask;
            bool pin_is_set = nrf_drv_gpiote_in_is_set(p_btn->pin_no);
            if ((m_pin_state & (1 << p_btn->pin_no)) == (pin_is_set << p_btn->pin_no))
            {
                uint32_t transition = !(pin_is_set ^ (p_btn->active_state == APP_BUTTON_ACTIVE_HIGH));

                if (p_btn->button_handler)
                {
                    p_btn->button_handler(p_btn->pin_no, transition);
                }
            }
        }
    }
}

static void gpiote_event_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    uint32_t err_code;
    uint32_t pin_mask = 1 << pin;

    // Start detection timer. If timer is already running, the detection period is restarted.
    // NOTE: Using the p_context parameter of app_timer_start() to transfer the pin states to the
    //       timeout handler (by casting event_pins_mask into the equally sized void * p_context
    //       parameter).
    err_code = app_timer_stop(m_detection_delay_timer_id);
    if (err_code != NRF_SUCCESS)
    {
        // The impact in app_button of the app_timer queue running full is losing a button press.
        // The current implementation ensures that the system will continue working as normal.
        return;
    }

    if (!(m_pin_transition & pin_mask))
    {
        if (nrf_drv_gpiote_in_is_set(pin))
        {
            m_pin_state |= pin_mask;
        }
        else
        {
            m_pin_state &= ~(pin_mask);
        }
        m_pin_transition |= (pin_mask);

        err_code = app_timer_start(m_detection_delay_timer_id, m_detection_delay, NULL);
        if (err_code != NRF_SUCCESS)
        {
            // The impact in app_button of the app_timer queue running full is losing a button press.
            // The current implementation ensures that the system will continue working as normal.
        }
    }
    else
    {
        m_pin_transition &= ~pin_mask;
    }
}

uint32_t app_button_init(app_button_cfg_t *             p_buttons,
                         uint8_t                        button_count,
                         uint32_t                       detection_delay)
{
    uint32_t err_code;
    
    if (detection_delay < APP_TIMER_MIN_TIMEOUT_TICKS)
    {
        return NRF_ERROR_INVALID_PARAM;
    }

    if (!nrf_drv_gpiote_is_init())
    {
        err_code = nrf_drv_gpiote_init();
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Save configuration.
    mp_buttons          = p_buttons;
    m_button_count      = button_count;
    m_detection_delay   = detection_delay;

    m_pin_state      = 0;
    m_pin_transition = 0;
    
    while (button_count--)
    {
        app_button_cfg_t * p_btn = &p_buttons[button_count];

        nrf_drv_gpiote_in_config_t config = GPIOTE_CONFIG_IN_SENSE_TOGGLE(false);
        config.pull = p_btn->pull_cfg;
        
        err_code = nrf_drv_gpiote_in_init(p_btn->pin_no, &config, gpiote_event_handler);
        if (err_code != NRF_SUCCESS)
        {
            return err_code;
        }
    }

    // Create polling timer.
    return app_timer_create(&m_detection_delay_timer_id,
                            APP_TIMER_MODE_SINGLE_SHOT,
                            detection_delay_timeout_handler);
}

uint32_t app_button_enable(void)
{
    ASSERT(mp_buttons);

    uint32_t i;
    for (i = 0; i < m_button_count; i++)
    {
        nrf_drv_gpiote_in_event_enable(mp_buttons[i].pin_no, true);
    }

    return NRF_SUCCESS;
}


uint32_t app_button_disable(void)
{
    ASSERT(mp_buttons);

    uint32_t i;
    for (i = 0; i < m_button_count; i++)
    {
       nrf_drv_gpiote_in_event_disable(mp_buttons[i].pin_no);
    }

    // Make sure polling timer is not running.
    return app_timer_stop(m_detection_delay_timer_id);
}


uint32_t app_button_is_pushed(uint8_t button_id, bool * p_is_pushed)
{
    ASSERT(button_id <= m_button_count);
    ASSERT(mp_buttons != NULL);

    app_button_cfg_t * p_btn = &mp_buttons[button_id];
    bool is_set = nrf_drv_gpiote_in_is_set(p_btn->pin_no);

    *p_is_pushed = !(is_set^(p_btn->active_state == APP_BUTTON_ACTIVE_HIGH));
    
    return NRF_SUCCESS;
}
