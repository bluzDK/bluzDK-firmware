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
#include "i2c_hal.h"
#include "gpio_hal.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "nrf.h"
#include "hw_config.h"
#include "hw_layout.h"
#include "nrf_drv_twi.h"
#include "nrf51_driver_config.h"

const nrf_drv_twi_t     p_twi_instance = NRF_DRV_TWI_INSTANCE(1);
nrf_drv_twi_config_t    p_twi_config = NRF_DRV_TWI_DEFAULT_CONFIG(1);
uint8_t dataOutBuffer[32];
uint8_t dataOutBufferSize = 0;

uint8_t dataInBuffer[32];
uint8_t dataInBufferSize = 0, dataInBufferStart = 0;

uint8_t dataAddress;
bool wireConfigured = false;

void twi_event_handler(nrf_drv_twi_evt_t *p_event);

void HAL_I2C_Init(HAL_I2C_Interface i2c, void* reserved)
{
}

void HAL_I2C_Begin(HAL_I2C_Interface i2c, I2C_Mode mode, uint8_t address, void* reserved)
{
    if (!wireConfigured) {
        p_twi_config.scl = TWI1_CONFIG_SCL;
        p_twi_config.sda = TWI1_CONFIG_SDA;
        int ret_code = nrf_drv_twi_init(&p_twi_instance, &p_twi_config,
                                        NULL); // Initiate twi driver with instance and configuration values
        APP_ERROR_CHECK(ret_code); // Check for errors in return value
        HW_ONE_CONFIG = HW1_TWI;
        nrf_drv_twi_enable(&p_twi_instance); // Enable the TWI instance
        wireConfigured = true;
    }

}

void HAL_I2C_End(HAL_I2C_Interface i2c, void* reserved)
{
    nrf_drv_twi_disable(&p_twi_instance); // Disable the TWI instance
    nrf_drv_twi_uninit(&p_twi_instance); // Uninit the TWI instance
    wireConfigured = false;
}

void HAL_I2C_Set_Speed(HAL_I2C_Interface i2c, uint32_t speed, void* reserved)
{
    if (speed == 100000) {
        p_twi_config.frequency = NRF_TWI_FREQ_100K;
    } else if (speed == 250000) {
        p_twi_config.frequency = NRF_TWI_FREQ_250K;
    } else if (speed == 400000) {
        p_twi_config.frequency = NRF_TWI_FREQ_400K;
    }
}

void HAL_I2C_Stretch_Clock(HAL_I2C_Interface i2c, bool stretch, void* reserved)
{
}

uint32_t HAL_I2C_Request_Data(HAL_I2C_Interface i2c, uint8_t address, uint8_t quantity, uint8_t stop,void* reserved)
{
    dataInBufferStart = 0;
    dataInBufferSize = quantity;
    if (nrf_drv_twi_rx(&p_twi_instance, address, dataInBuffer, quantity, !stop) == NRF_SUCCESS) {
        return quantity;
    } else {
        return 0;
    }
}

void HAL_I2C_Begin_Transmission(HAL_I2C_Interface i2c, uint8_t address,void* reserved)
{
    dataAddress = address;
}

uint8_t HAL_I2C_End_Transmission(HAL_I2C_Interface i2c, uint8_t stop,void* reserved)
{
    uint8_t err_code = nrf_drv_twi_tx(&p_twi_instance, dataAddress, dataOutBuffer, dataOutBufferSize, !stop);
    dataOutBufferSize = 0;
    return err_code;
}


uint32_t HAL_I2C_Write_Data(HAL_I2C_Interface i2c, uint8_t data,void* reserved)
{
    dataOutBuffer[dataOutBufferSize++] = data;
    return dataOutBufferSize;
}

int32_t HAL_I2C_Available_Data(HAL_I2C_Interface i2c,void* reserved)
{
    return dataInBufferSize;
}

int32_t HAL_I2C_Read_Data(HAL_I2C_Interface i2c,void* reserved)
{
    dataInBufferSize--;
    return dataInBuffer[dataInBufferStart++];
}

int32_t HAL_I2C_Peek_Data(HAL_I2C_Interface i2c,void* reserved)
{
    return dataInBuffer[dataInBufferStart];
}

void HAL_I2C_Flush_Data(HAL_I2C_Interface i2c,void* reserved)
{
  // XXX: to be implemented.
}

bool HAL_I2C_Is_Enabled(HAL_I2C_Interface i2c,void* reserved)
{
    return NRF_TWI1->ENABLE;
}

void HAL_I2C_Set_Callback_On_Receive(HAL_I2C_Interface i2c, void (*function)(int),void* reserved)
{

}

void HAL_I2C_Set_Callback_On_Request(HAL_I2C_Interface i2c, void (*function)(void),void* reserved)
{

}

/*******************************************************************************
 * Function Name  : HAL_I2C1_EV_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function handles I2C1 Event interrupt request(Only for Slave mode).
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_I2C1_EV_Handler(void)
{
}

/*******************************************************************************
 * Function Name  : HAL_I2C1_ER_Handler (Declared as weak in stm32_it.cpp)
 * Description    : This function handles I2C1 Error interrupt request.
 * Input          : None.
 * Output         : None.
 * Return         : None.
 *******************************************************************************/
void HAL_I2C1_ER_Handler(void)
{
}

void HAL_I2C_Enable_DMA_Mode(HAL_I2C_Interface i2c, bool enable,void* reserved)
{
}

void twi_event_handler(nrf_drv_twi_evt_t *p_event)
{
    
}
