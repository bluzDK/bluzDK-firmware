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
    if (data_callback != NULL) {
        data_callback(data, length);
    }
    return 1;
}

void CustomDataService::registerCallback(void (*dc)(uint8_t *data, uint16_t length))
{
    data_callback = dc;
}

void CustomDataService::sendData(uint8_t *data, uint16_t length)
{
    uint8_t rsp[length+1];
    rsp[0] = CUSTOM_DATA_SERVICE & 0xFF;
    memcpy(rsp+1, data, length);

    DataManagementLayer::sendData(length+1, rsp);
}

void customDataServiceRegisterCallback(void (*data_callback)(uint8_t *data, uint16_t length))
{
    CustomDataService::instance()->registerCallback(data_callback);
}

void customDataServiceSendData(uint8_t *data, uint16_t length)
{
    CustomDataService::instance()->sendData(data, length);
}