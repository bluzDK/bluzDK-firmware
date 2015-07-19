#ifndef _STREAM_MANAGER_H
#define	_STREAM_MANAGER_Hx

#ifdef __cplusplus

#include <stdint.h>
#include "stream_hal.h"

class StreamManager
{
    public:
        StreamManager();
        StreamManager(int bufferSize);
    
        int32_t connect(const sockaddr_b *addr, long addrlen);
        int32_t send(uint32_t sockid, const void* buffer, uint32_t len);
        int32_t receive(uint32_t sockid, void* buffer, uint32_t len, unsigned long _timeout);
        int32_t close(uint32_t sockid);
    
        static const int32_t MAX_NUMBER_OF_SOCKETS = 2;

    
    private:
        Stream streams[MAX_NUMBER_OF_SOCKETS];
        uint32_t id;
};

#endif
#endif	/* STREAM_MANAGER_H */