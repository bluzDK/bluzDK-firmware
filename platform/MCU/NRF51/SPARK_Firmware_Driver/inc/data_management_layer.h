#ifndef _DATA_MANAGEMENT_LAYER_H
#define	_DATA_MANAGEMENT_LAYER_H

#include <stdint.h>
#include "data_service.h"

static const int32_t MAX_NUMBER_OF_SERVICES = 32;
#ifdef __cplusplus
extern "C" {
#endif
    void dataManagementFeedData(uint16_t id, int16_t length, uint8_t *data);
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
    
    static void feedData(uint16_t id, int16_t length, uint8_t *data);
    
private:
    static int16_t dataServicesRegistered;
    static DataService* services[MAX_NUMBER_OF_SERVICES];
};

#endif
#endif	/* _DATA_MANAGEMENT_LAYER_H */