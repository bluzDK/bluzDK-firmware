#ifndef _DATA_SERVICE_H
#define	_DATA_SERVICE_H

#ifdef __cplusplus

#include <stdint.h>

class DataService
{
public:
    virtual int32_t getServiceID();
    virtual int32_t DataCallback(uint8_t *data, int16_t length);
};

#endif
#endif	/* _DATA_SERVICE_H */