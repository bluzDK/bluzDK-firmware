#include <string.h>
#include "custom_data_service.h"
#include "data_management_layer.h"
#include "registered_data_services.h"
#include "deviceid_hal.h"
#include "nrf_gpio.h"

CustomDataService* CustomDataService::m_pInstance = NULL;

CustomDataService* CustomDataService::instance()
{
    if (!m_pInstance){   // Only allow one instance of class to be generated.
        m_pInstance = new CustomDataService;
    }
    return m_pInstance;

}

//DataService functions
int32_t CustomDataService::getServiceID()
{
    return CUSTOM_DATA_SERVICE;
}
int32_t CustomDataService::DataCallback(uint8_t *data, int16_t length)
{
    nrf_gpio_cfg_output(30);
    nrf_gpio_pin_set(30);
    return 1;
}