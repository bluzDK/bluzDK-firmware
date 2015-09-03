#ifndef _DATA_SERVICE_H
#define	_DATA_SERVICE_H

#ifdef __cplusplus

#include <stdint.h>

class DataService
{
public:
    DataService();
    virtual int32_t getServiceID() { return 0; };
    virtual int32_t DataCallback(uint8_t *data, int16_t length) { return 0; };
};

#endif
#endif	/* _DATA_SERVICE_H */