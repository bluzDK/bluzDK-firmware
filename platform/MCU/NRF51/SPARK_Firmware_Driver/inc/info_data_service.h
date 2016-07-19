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

#ifndef _INFO_DATA_SERVICE_H
#define	_INFO_DATA_SERVICE_H

#ifdef __cplusplus

#include <stdint.h>
#include <stdlib.h>
#include "data_service.h"

//Info Command Request
typedef enum {
    GET_ID=0,
    SET_MODE,
    SET_CONNECTION_PARAMETERS
} INFO_COMMAND;


class InfoDataService : public DataService
{
public:
    static InfoDataService* instance();
    
    //DataService functions
    virtual int32_t getServiceID();
    virtual int32_t DataCallback(uint8_t *data, int16_t length);
    void registerCallback(void (*ec)(uint8_t event, uint8_t *data, uint16_t length));
    
private:
    //this is a singleton class, so these all need to be private so they can't be called
    InfoDataService(){};
    InfoDataService(InfoDataService const&){};
    InfoDataService& operator=(InfoDataService const&);
    static InfoDataService* m_pInstance;
    void (*event_callback)(uint8_t event, uint8_t *data, uint16_t length);
};

#endif
#endif	/* _INFO_DATA_SERVICE_H */

#ifdef __cplusplus
extern "C" {
#endif
void infoDataServiceRegisterCallback(void (*event_callback)(uint8_t event, uint8_t *data, uint16_t length));
#ifdef __cplusplus
}
#endif