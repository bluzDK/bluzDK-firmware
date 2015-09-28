#include <stdint.h>
#include <cstdlib>
#include "data_management_layer.h"
extern "C" {
#include "particle_data_service.h"
}

int16_t DataManagementLayer::dataServicesRegistered = 0;
DataService* DataManagementLayer::services[MAX_NUMBER_OF_SERVICES] = {NULL};

DataManagementLayer::DataManagementLayer() { dataServicesRegistered=0; }

void DataManagementLayer::registerService(DataService* service)
{
    services[dataServicesRegistered++] = service;
}

void DataManagementLayer::feedData(uint16_t id, int16_t length, uint8_t *data)
{
    for (int i = 0; i < dataServicesRegistered; i++)
    {
        if (services[i] != NULL && id == services[i]->getServiceID()) {
            services[i]->DataCallback(data, length);
            break;
        }
    }
}

void DataManagementLayer::sendData(int16_t length, uint8_t *data)
{
//a bit of a hack for now, should HAL this out, but it'll work for the time being
#ifdef BLUZ
    particle_service_send_data(data, length);
#endif
}

void dataManagementFeedData(uint16_t id, int16_t length, uint8_t *data)
{
    DataManagementLayer::feedData(id, length, data);
}
    
void dataManagementSendData(int16_t length, uint8_t *data)
{
    DataManagementLayer::sendData(length, data);
}

void dataManagementRegisterService(DataService* service)
{
    DataManagementLayer::registerService(service);
}