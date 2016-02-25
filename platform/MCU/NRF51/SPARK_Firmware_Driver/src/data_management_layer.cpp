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
#if PLATFORM_ID==103
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