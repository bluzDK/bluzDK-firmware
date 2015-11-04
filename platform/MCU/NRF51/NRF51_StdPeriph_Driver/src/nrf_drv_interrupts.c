/* Copyright (c) 2015 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "nrf_drv_twi.h"
#include "spi_master.h"

void nrf_drv_spi0_int_handler(void);
void nrf_drv_twi0_int_handler(void);

void SPI0_TWI0_IRQHandler(void)
{
    if (NRF_SPI0->ENABLE) {
        if ((NRF_SPI0->EVENTS_READY == 1) && (NRF_SPI0->INTENSET & SPI_INTENSET_READY_Msk))
        {
            NRF_SPI0->EVENTS_READY = 0;
            nrf_drv_spi0_int_handler();
        }
    } else if (NRF_TWI0->ENABLE) {
        nrf_drv_twi0_int_handler();
    }
}
