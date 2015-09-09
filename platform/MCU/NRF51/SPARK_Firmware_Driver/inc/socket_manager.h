#ifndef _SOCKET_MANAGER_H
#define	_SOCKET_MANAGER_H

#ifdef __cplusplus

#include <stdint.h>
#include "socket.h"

class SocketManager : public DataService
{
public:
    SocketManager();
    
    static uint32_t create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif);
    static int32_t connect(uint32_t sd, const sockaddr_b *addr, long addrlen);
    static int32_t send(uint32_t sockid, const void* buffer, uint32_t len);
    static int32_t receive(uint32_t sockid, void* buffer, uint32_t len, unsigned long _timeout);
    static int32_t close(uint32_t sockid);
    
    static const int32_t MAX_NUMBER_OF_SOCKETS = 2;
    
    
private:
    static Socket sockets[MAX_NUMBER_OF_SOCKETS];
};

#endif
#endif	/* _SOCKET_MANAGER_H */

