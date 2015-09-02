#include <stdint.h>
#include "data_management_layer.h"

DataManagementLayer::DataManagementLayer() { dataServicesRegistered=0; }

void DataManagementLayer::registerService(DataService service)
{
    services[dataServicesRegistered++] = service;
}

void DataManagementLayer::feedData(uint16_t id, int16_t length, uint8_t *data)
{
    for (int i = 0; i < dataServicesRegistered; i++)
    {
        if (id == services[i].getServiceID()) {
            services[i].DataCallback(data, length);
            break;
        }
    }
}

void DataManagementLayer::sendData(uint8_t *data, int16_t length)
{
//#ifdef BLUZ
//    scs_data_send
//#endif
}