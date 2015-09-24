
#include "rgbled_hal.h"
#include "gpio_hal.h"
#include "pinmap_impl.h"
#undef SCK
#undef MOSI
#undef MISO
#undef SS
#include "nrf_gpio.h"

void Set_RGB_LED_Values(uint16_t r, uint16_t g, uint16_t b)
{
    if (r > 0x00) {
        nrf_gpio_pin_clear(PIN_MAP[RGBR].gpio_pin);
    } else {
        nrf_gpio_pin_set(PIN_MAP[RGBR].gpio_pin);
    }
    
    if (g > 0x00) {
        nrf_gpio_pin_clear(PIN_MAP[RGBG].gpio_pin);
    } else {
        nrf_gpio_pin_set(PIN_MAP[RGBG].gpio_pin);
    }
    
    if (b > 0x00) {
        nrf_gpio_pin_clear(PIN_MAP[RGBB].gpio_pin);
    } else {
        nrf_gpio_pin_set(PIN_MAP[RGBB].gpio_pin);
    }
}

void Get_RGB_LED_Values(uint16_t* rgb)
{

}

void Set_User_LED(uint8_t state)
{

}

void Toggle_User_LED()
{

}

uint16_t Get_RGB_LED_Max_Value()
{
    return 65535;
}
