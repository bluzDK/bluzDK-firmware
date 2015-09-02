#ifndef _DATA_MANAGEMENT_LAYER_H
#define	_DATA_MANAGEMENT_LAYER_H

#ifdef __cplusplus

#include <stdint.h>
#include "data_service.h"

class DataManagementLayer
{
public:
    DataManagementLayer();
    static void registerService(DataService service);
    static void sendData(uint8_t *data, int16_t length);
    
    static void feedData(uint16_t id, int16_t length, uint8_t *data);
    static const int32_t MAX_NUMBER_OF_SERVICES = 32;
    
private:
    static int16_t dataServicesRegistered;
    static DataService services[MAX_NUMBER_OF_SERVICES];
};

#endif
#endif	/* _DATA_MANAGEMENT_LAYER_H */