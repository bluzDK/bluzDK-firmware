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
#include "socket.h"
extern "C" {
#include "particle_data_service.h"
#include "hw_gateway_config.h"
#include "spi_slave_stream.h"
}

#include "debug.h"

int16_t DataManagementLayer::dataServicesRegistered = 0;
DataService* DataManagementLayer::services[MAX_NUMBER_OF_SERVICES] = {NULL};

DataManagementLayer::DataManagementLayer() { dataServicesRegistered=0; }

void DataManagementLayer::registerService(DataService* service)
{
    services[dataServicesRegistered++] = service;
}

void DataManagementLayer::feedData(int16_t length, uint8_t *data)
{
    uint16_t serviceID = 0x00 | data[0];
    for (int i = 0; i < dataServicesRegistered; i++)
    {
        if (services[i] != NULL && serviceID == services[i]->getServiceID()) {
            services[i]->DataCallback(data+1, length-1);
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
#if PLATFORM_ID==269
    data[0] = (( (length-BLE_HEADER_SIZE-SPI_HEADER_SIZE) & 0xFF00) >> 8);
    data[1] = ( (length-BLE_HEADER_SIZE-SPI_HEADER_SIZE) & 0xFF);
    data[2] = GATEWAY_ID;

    spi_slave_send_data(data, length);
#endif
}

void dataManagementFeedData(int16_t length, uint8_t *data)
{
    DataManagementLayer::feedData(length, data);
}
    
void dataManagementSendData(int16_t length, uint8_t *data)
{
    DataManagementLayer::sendData(length, data);
}

void dataManagementRegisterService(DataService* service)
{
    DataManagementLayer::registerService(service);
}