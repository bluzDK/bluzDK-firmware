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
#include <string.h>
#include "info_data_service.h"
#include "data_management_layer.h"
#include "registered_data_services.h"
#include "deviceid_hal.h"

InfoDataService* InfoDataService::m_pInstance = NULL;

InfoDataService* InfoDataService::instance()
{
    if (!m_pInstance) {  // Only allow one instance of class to be generated.
        m_pInstance = new InfoDataService;
    }
    return m_pInstance;

}

//DataService functions
int32_t InfoDataService::getServiceID()
{
    return INFO_DATA_SERVICE;
}
int32_t InfoDataService::DataCallback(uint8_t *data, int16_t length)
{
    int offset = 0;
#if PLATFORM_ID==269
    offset = 3;
#endif

    switch (data[0]) {
        case GET_ID:
            uint8_t id[12];
            HAL_device_ID(id, 12);
            
            uint8_t rsp[13+offset];
            rsp[0+offset] = INFO_DATA_SERVICE & 0xFF;
            memcpy(rsp+1+offset, id, 12);
            
            DataManagementLayer::sendData(13+offset, rsp);
            break;
    }
    return 1;
}

