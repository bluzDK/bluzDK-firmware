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

#include "adc_hal.h"
#include "pinmap_impl.h"
#undef MISO
#undef MOSI
#undef SCK
#undef SS
#include "nrf_soc.h"
#include "nrf_adc.h"

volatile int32_t adc_sample = -1;
/**
 * @brief ADC interrupt handler.
 */
void ADC_IRQHandler(void)
{
    nrf_adc_conversion_event_clean();
    adc_sample = nrf_adc_result_get();
    nrf_adc_stop();
}



void HAL_ADC_Set_Sample_Time(uint8_t ADC_SampleTime)
{
}

/*
 * @brief Read the analog value of a pin.
 * Should return a 16-bit value, 0-1024 (0 = LOW, 1024 = HIGH)
 * Note: ADC is 10-bit. Currently it returns 0-1024
 */
int32_t HAL_ADC_Read(uint16_t pin)
{
    while (nrf_adc_is_busy()) { }
    
    const nrf_adc_config_t nrf_adc_config = NRF_ADC_CONFIG_DEFAULT;
    adc_sample = -1;
    
    nrf_adc_config_input_t adcPin = NRF_ADC_CONFIG_INPUT_DISABLED;
    switch (pin) {
        case A0:
            adcPin = NRF_ADC_CONFIG_INPUT_7;
            break;
        case A1:
            adcPin = NRF_ADC_CONFIG_INPUT_6;
            break;
        case A2:
            adcPin = NRF_ADC_CONFIG_INPUT_5;
            break;
        case A3:
            adcPin = NRF_ADC_CONFIG_INPUT_4;
            break;
        case A4:
            adcPin = NRF_ADC_CONFIG_INPUT_3;
            break;
        case A5:
            adcPin = NRF_ADC_CONFIG_INPUT_2;
            break;
        default:
            break;
    }
    
    // Initialize and configure ADC
    nrf_adc_configure( (nrf_adc_config_t *)&nrf_adc_config);
    nrf_adc_input_select(adcPin);
    nrf_adc_int_enable(ADC_INTENSET_END_Enabled << ADC_INTENSET_END_Pos);
    NVIC_SetPriority(ADC_IRQn, NRF_APP_PRIORITY_HIGH);
    NVIC_EnableIRQ(ADC_IRQn);
    nrf_adc_start();
    
    while (adc_sample == -1) { }
    return adc_sample;
}

/*
 * @brief Initialize the ADC peripheral.
 */
void HAL_ADC_DMA_Init()
{
}
