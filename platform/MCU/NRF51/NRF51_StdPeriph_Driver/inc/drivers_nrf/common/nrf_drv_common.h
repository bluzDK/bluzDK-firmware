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

#ifndef NRF_DRV_COMMON_H__
#define NRF_DRV_COMMON_H__

#include "nrf.h"
#include <stdint.h>

/**
 * @enum nrf_drv_state_t
 * @brief Driver state.
 */
typedef enum 
{ 
	NRF_DRV_STATE_UNINITIALIZED, /**< Uninitialized. */
	NRF_DRV_STATE_INITIALIZED, /**< Initialized but powered off. */
	NRF_DRV_STATE_POWERED_ON
} nrf_drv_state_t;

/**
 * @enum nrf_drv_pwr_ctrl_t
 * @brief Driver power state selection.
 */
typedef enum
{
    NRF_DRV_PWR_CTRL_ON,   /**< Power on request. */
    NRF_DRV_PWR_CTRL_OFF   /**< Power off request. */
} nrf_drv_pwr_ctrl_t;

/**
 * @brief Function sets priority and enables NVIC interrupt
 *
 * @note Function checks if correct priority is used when softdevice is present
 *
 * @param[in] IRQn     Interrupt id
 * @param[in] priority Interrupt priority
 */
void nrf_drv_common_irq_enable(IRQn_Type IRQn, uint8_t priority);

/**
 * @brief Function disables NVIC interrupt
 *
 * @param[in] IRQn     Interrupt id
 */
__STATIC_INLINE void nrf_drv_common_irq_disable(IRQn_Type IRQn);

#ifndef SUPPRESS_INLINE_IMPLEMENTATION
__STATIC_INLINE void nrf_drv_common_irq_disable(IRQn_Type IRQn)
{
    NVIC_DisableIRQ(IRQn);
}
#endif

#endif //NRF_DRV_COMMON_H__
