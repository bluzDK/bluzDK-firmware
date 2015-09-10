#include <stdint.h>
#include <cstdlib>
#include "data_management_layer.h"

int16_t DataManagementLayer::dataServicesRegistered = 0;
DataService DataManagementLayer::services[MAX_NUMBER_OF_SERVICES] = {DataService()};

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

void DataManagementLayer::sendData(int16_t length, uint8_t *data)
{
//#ifdef BLUZ
//    scs_data_send
//#endif
}

void dataManagementFeedData(uint16_t id, int16_t length, uint8_t *data)
{
    DataManagementLayer::feedData(id, length, data);
}
    
void dataManagementSendData(int16_t length, uint8_t *data)
{
    DataManagementLayer::sendData(length, data);
}

void dataManagementRegisterService(DataService service)
{
    DataManagementLayer::registerService(service);
}