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

    void registerCallback(void (*dc)(uint8_t *data, uint16_t length));
    void sendData(uint8_t *data, uint16_t length);

private:
    //this is a singleton class, so these all need to be private so they can't be called
    CustomDataService(){ };
    CustomDataService(CustomDataService const&){ };
    CustomDataService& operator=(CustomDataService const&);
    static CustomDataService* m_pInstance;
    void (*data_callback)(uint8_t *m_tx_buf, uint16_t size);
};

#endif
#endif	/* _CUSTOM_DATA_SERVICE_H */

#ifdef __cplusplus
extern "C" {
#endif
void customDataServiceRegisterCallback(void (*data_callback)(uint8_t *data, uint16_t length));
void customDataServiceSendData(uint8_t *data, uint16_t length);
#ifdef __cplusplus
}
#endif