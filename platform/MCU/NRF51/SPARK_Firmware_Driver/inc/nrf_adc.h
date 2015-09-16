#ifndef __QLIB_ADC_H__
#define __QLIB_ADC_H__

#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"

typedef enum
{
    ADC_RES_8bit = 0, 
    ADC_RES_9bit, 
    ADC_RES_10bit
} ADC_Res_t;

typedef enum 
{
    ADC_INPUT_AIN0_P26 = 0, 
    ADC_INPUT_AIN1_P27 = 1, 
    ADC_INPUT_AIN2_P01 = 2, 
    ADC_INPUT_AIN3_P02 = 3, 
    ADC_INPUT_AIN4_P03 = 4, 
    ADC_INPUT_AIN5_P04 = 5, 
    ADC_INPUT_AIN6_P05 = 6, 
    ADC_INPUT_AIN7_P06 = 7
} ADC_input_selection_t;

typedef enum 
{
    ADC_INT_DISABLED = 0,
    ADC_INT_ENABLED
} ADC_interrupt_enabled_t;

/**
 * @brief Configures the ADC to measure an analog input relative to VDD. 
 *
 * @param ADC_res specifies the resolution of the ADC
 *
 * @param ADC_input_selection specifies the analog input pin to the ADC
 *
 * @param ADC_interrupt_enabled enables the ADC interrupt
 *
 */

void nrf_adc_init(ADC_Res_t ADC_res, ADC_input_selection_t ADC_input_selection, ADC_interrupt_enabled_t ADC_interrupt_enabled);

/**
 * @brief Configures the ADC to measure VBAT. 
 *
 */
 
void nrf_adc_init_vbat(void);

/**
 * @brief Reads a single sample from the ADC without using interrupts, should only be used if the ADC is initialized without interrupts 
 *
 * @return Returns the ADC sample
 */

uint32_t nrf_adc_read(void);

/**
 * @brief Reads VBAT and converts it to a floating point number with a unit of 1.0 V
 *
 * @return VBAT [V]
 */

float nrf_adc_read_vbat_f(void);

/**
 * @brief Reads VBAT and converts it to an integer with a unit of 1 mV
 *
 * @return VBAT [mV]
 */

uint32_t nrf_adc_read_vbat_mv(void);

// ADC INTERRUPT EXAMPLE
// Remember to include something like this in the code if ADC_interrupt_enabled is set
//
// void ADC_IRQHandler()
// {
//     NRF_ADC->EVENTS_END = 0;
//     // The data can be read from NRF_ADC->RESULT
// }

#endif
