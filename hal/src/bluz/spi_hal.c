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
#include "spi_hal.h"
#include "pinmap_impl.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "hw_config.h"
#include "spi_master_fast.h"
#include "nrf51_driver_config.h"

SPI_config_t spi_config =  {
    .pin_SCK                 = SPIM1_SCK_PIN,
    .pin_MOSI                = SPIM1_MOSI_PIN,
    .pin_MISO                = SPIM1_MISO_PIN,
    .pin_CSN                 = SPIM1_SS_PIN,
    .frequency               = SPI_FREQ_1MBPS,
    .config.fields.mode      = SPI_MODE3,
    .config.fields.bit_order = SPI_BITORDER_MSB_LSB
};

void reset_spi_config(void)
{
    spi_master_init(SPI1, &spi_config);
}

void HAL_SPI_Init(HAL_SPI_Interface spi)
{
    
}

void HAL_SPI_Begin(HAL_SPI_Interface spi, uint16_t pin)
{
    HW_ONE_CONFIG = HW1_SPI;
    if (pin != SPI_DEFAULT_SS) {
        spi_config.pin_CSN = PIN_MAP[pin].gpio_pin;
    }
    spi_master_init(SPI1, &spi_config);
}

void HAL_SPI_Begin_Ext(HAL_SPI_Interface spi, SPI_Mode mode, uint16_t pin, void* reserved)
{
}

void HAL_SPI_End(HAL_SPI_Interface spi)
{
    //TO DO: Need an ending
    spi_master_disable(SPI1);
}

void HAL_SPI_Set_Bit_Order(HAL_SPI_Interface spi, uint8_t order)
{
    if (order == LSBFIRST) {
        spi_config.config.fields.bit_order = SPI_BITORDER_LSB_MSB;
    } else {
        spi_config.config.fields.bit_order = SPI_BITORDER_MSB_LSB;
    }
    reset_spi_config();
}

void HAL_SPI_Set_Data_Mode(HAL_SPI_Interface spi, uint8_t mode)
{
    switch (mode) {
        case SPI_MODE0:
            spi_config.config.fields.mode = SPI_MODE_ZERO;
            break;
        case SPI_MODE1:
            spi_config.config.fields.mode = SPI_MODE_ONE;
            break;
        case SPI_MODE2:
            spi_config.config.fields.mode = SPI_MODE_TWO;
            break;
        case SPI_MODE3:
            spi_config.config.fields.mode = SPI_MODE_THREE;
            break;
    }
    reset_spi_config();
}

void HAL_SPI_Set_Clock_Divider(HAL_SPI_Interface spi, uint8_t rate)
{
    switch (rate)
    {
        case SPI_CLOCK_DIV2:
            spi_config.frequency = SPI_FREQ_8MBPS;
            break;
        case SPI_CLOCK_DIV4:
            spi_config.frequency = SPI_FREQ_4MBPS;
            break;
        case SPI_CLOCK_DIV8:
            spi_config.frequency = SPI_FREQ_2MBPS;
            break;
        case SPI_CLOCK_DIV16:
            spi_config.frequency = SPI_FREQ_1MBPS;
            break;
        case SPI_CLOCK_DIV32:
            spi_config.frequency = SPI_FREQ_500KBPS;
            break;
        case SPI_CLOCK_DIV64:
            spi_config.frequency = SPI_FREQ_250KBPS;
            break;
        case SPI_CLOCK_DIV128:
            spi_config.frequency = SPI_FREQ_125KBPS;
            break;
            
    }
    reset_spi_config();
}

uint16_t HAL_SPI_Send_Receive_Data(HAL_SPI_Interface spi, uint16_t data)
{
    uint8_t rx_data[1];
    rx_data[0] = 0x00;
    
    spi_master_tx_rx(SPI1, 1, (uint8_t *)&data, rx_data);
    return rx_data[0];
}

bool HAL_SPI_Is_Enabled(HAL_SPI_Interface spi)
{
    return NRF_SPI1->ENABLE;
}

void HAL_SPI_DMA_Transfer(HAL_SPI_Interface spi, void* tx_buffer, void* rx_buffer, uint32_t length, HAL_SPI_DMA_UserCallback userCallback)
{
    
}

void HAL_SPI_Info(HAL_SPI_Interface spi, hal_spi_info_t* info, void* reserved)
{
}

bool HAL_SPI_Is_Enabled_Old()
{
    return false;
}

void HAL_SPI_Set_Callback_On_Select(HAL_SPI_Interface spi, HAL_SPI_Select_UserCallback cb, void* reserved)
{
}

void HAL_SPI_DMA_Transfer_Cancel(HAL_SPI_Interface spi)
{
}

int32_t HAL_SPI_DMA_Transfer_Status(HAL_SPI_Interface spi, HAL_SPI_TransferStatus* st)
{
    return 0;
}