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
#include "spi_master.h"
#include "nrf51_driver_config.h"

spi_master_config_t spi_config = SPI_MASTER_INIT_DEFAULT;
volatile bool spi0_transmission_completed = false;

void spi0_master_event_handler(spi_master_evt_t spi_master_evt)
{
    switch (spi_master_evt.evt_type)
    {
        case SPI_MASTER_EVT_TRANSFER_COMPLETED:
            //Data transmission is ended successful. 'rx_buffer' has data received from SPI slave.
            
            spi0_transmission_completed = true;
            break;
            
        default:
            //No implementation needed.
            break;
    }
}

void reset_spi_config(void)
{
    NRF_SPI0->ENABLE = (SPI_ENABLE_ENABLE_Disabled << SPI_ENABLE_ENABLE_Pos);
    
    NRF_SPI0->CONFIG = (uint32_t)(spi_config.SPI_CONFIG_CPHA << SPI_CONFIG_CPHA_Pos) | (spi_config.SPI_CONFIG_CPOL << SPI_CONFIG_CPOL_Pos) | (spi_config.SPI_CONFIG_ORDER << SPI_CONFIG_ORDER_Pos);
    NRF_SPI0->FREQUENCY = spi_config.SPI_Freq;
    
    NRF_SPI0->ENABLE = (SPI_ENABLE_ENABLE_Enabled << SPI_ENABLE_ENABLE_Pos);
}

void HAL_SPI_Init(HAL_SPI_Interface spi)
{
    spi_config.SPI_Pin_SCK = SPIM0_SCK_PIN;
    spi_config.SPI_Pin_MISO = SPIM0_MISO_PIN;
    spi_config.SPI_Pin_MOSI = SPIM0_MOSI_PIN;
    spi_config.SPI_Pin_SS = SPIM0_SS_PIN;
    spi_config.SPI_CONFIG_ORDER = SPI_CONFIG_ORDER_MsbFirst;
    spi_config.SPI_PriorityIRQ = APP_IRQ_PRIORITY_HIGH;
}

void HAL_SPI_Begin(HAL_SPI_Interface spi, uint16_t pin)
{
    HW_ZERO_CONFIG = HW0_SPI;
    spi_config.SPI_Pin_SS = PIN_MAP[pin].gpio_pin;
    spi_master_open(SPI_MASTER_0, &spi_config);
    spi_master_evt_handler_reg(SPI_MASTER_0, spi0_master_event_handler);
}

void HAL_SPI_End(HAL_SPI_Interface spi)
{
    spi_master_close(SPI_MASTER_0);
}

void HAL_SPI_Set_Bit_Order(HAL_SPI_Interface spi, uint8_t order)
{
    if (order == LSBFIRST) {
        spi_config.SPI_CONFIG_ORDER = SPI_CONFIG_ORDER_LsbFirst;
    } else {
        spi_config.SPI_CONFIG_ORDER = SPI_CONFIG_ORDER_MsbFirst;
    }
    reset_spi_config();
}

void HAL_SPI_Set_Data_Mode(HAL_SPI_Interface spi, uint8_t mode)
{
    switch (mode) {
        case SPI_MODE0:
            spi_config.SPI_CONFIG_CPOL = SPI_CONFIG_CPOL_ActiveLow;
            spi_config.SPI_CONFIG_CPHA = SPI_CONFIG_CPHA_Trailing;
            break;
        case SPI_MODE1:
            spi_config.SPI_CONFIG_CPOL = SPI_CONFIG_CPOL_ActiveLow;
            spi_config.SPI_CONFIG_CPHA = SPI_CONFIG_CPHA_Leading;
            break;
        case SPI_MODE2:
            spi_config.SPI_CONFIG_CPOL = SPI_CONFIG_CPOL_ActiveHigh;
            spi_config.SPI_CONFIG_CPHA = SPI_CONFIG_CPHA_Trailing;
            break;
        case SPI_MODE3:
            spi_config.SPI_CONFIG_CPOL = SPI_CONFIG_CPOL_ActiveHigh;
            spi_config.SPI_CONFIG_CPHA = SPI_CONFIG_CPHA_Leading;
            break;
    }
    reset_spi_config();
}

void HAL_SPI_Set_Clock_Divider(HAL_SPI_Interface spi, uint8_t rate)
{
    switch (rate)
    {
        case SPI_CLOCK_DIV2:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_M8;
            break;
        case SPI_CLOCK_DIV4:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_M4;
            break;
        case SPI_CLOCK_DIV8:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_M2;
            break;
        case SPI_CLOCK_DIV16:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_M1;
            break;
        case SPI_CLOCK_DIV32:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_K500;
            break;
        case SPI_CLOCK_DIV64:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_K250;
            break;
        case SPI_CLOCK_DIV128:
            spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_K125;
            break;
            
    }
    reset_spi_config();
}

uint16_t HAL_SPI_Send_Receive_Data(HAL_SPI_Interface spi, uint16_t data)
{
    uint8_t rx_data[0];
    spi0_transmission_completed = false;
    
    uint32_t err_code = spi_master_send_recv(SPI_MASTER_0, (uint8_t *)&data, 1, rx_data, 1);
    if (err_code == NRF_SUCCESS)
    {
        while(spi0_transmission_completed == false) { }
    }
    else {
        
    }
    return rx_data[0];
}

bool HAL_SPI_Is_Enabled(HAL_SPI_Interface spi)
{
    return NRF_SPI0->ENABLE;
}

void HAL_SPI_DMA_Transfer(HAL_SPI_Interface spi, void* tx_buffer, void* rx_buffer, uint32_t length, HAL_SPI_DMA_UserCallback userCallback)
{
    
}

void HAL_SPI_Info(HAL_SPI_Interface spi, hal_spi_info_t* info, void* reserved)
{
}