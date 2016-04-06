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

#ifndef _DATA_MANAGEMENT_LAYER_H
#define	_DATA_MANAGEMENT_LAYER_H

#include <stdint.h>
#include "data_service.h"

#ifdef __cplusplus
extern "C" {
#endif

//#include "hw_config.h"

#ifdef __cplusplus
}
#endif


static const int32_t MAX_NUMBER_OF_SERVICES = 32;
#ifdef __cplusplus
extern "C" {
#endif
    void dataManagementFeedData(int16_t length, uint8_t *data);
    void dataManagementSendData(uint8_t *data, int16_t length);
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

class DataManagementLayer
{
public:
    DataManagementLayer();
    static void registerService(DataService* service);
    static void sendData(int16_t length, uint8_t *data);
    
    static void feedData(int16_t length, uint8_t *data);
    
private:
    static int16_t dataServicesRegistered;
    static DataService* services[MAX_NUMBER_OF_SERVICES];
};

#endif
#endif	/* _DATA_MANAGEMENT_LAYER_H */