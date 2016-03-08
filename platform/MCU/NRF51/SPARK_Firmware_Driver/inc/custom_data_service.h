#ifndef _CUSTOM_DATA_SERVICE_H
#define	_CUSTOM_DATA_SERVICE_H

#ifdef __cplusplus

#include <stdint.h>
#include <stdlib.h>
#include "data_service.h"

class CustomDataService : public DataService
{
public:
    static CustomDataService* instance();

    //DataService functions
    virtual int32_t getServiceID();
    virtual int32_t DataCallback(uint8_t *data, int16_t length);
    int32_t availableBytes;

private:
    //this is a singleton class, so these all need to be private so they can't be called
    CustomDataService(){
    	availableBytes = 0;
    };
    CustomDataService(CustomDataService const&){
    	availableBytes = 0;
    };
    CustomDataService& operator=(CustomDataService const&);
    static CustomDataService* m_pInstance;
};

#endif
#endif	/* _CUSTOM_DATA_SERVICE_H */