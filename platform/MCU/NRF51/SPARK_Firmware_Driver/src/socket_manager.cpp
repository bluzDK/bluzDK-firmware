
#include "socket_manager.h"

SocketManager::SocketManager() { }

uint32_t SocketManager::create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif)
{
    return -1;
}
int32_t SocketManager::connect(uint32_t sd, const sockaddr_b *addr, long addrlen)
{
    return -1;
}
int32_t SocketManager::send(uint32_t sockid, const void* buffer, uint32_t len) { return -1; }
int32_t SocketManager::receive(uint32_t sockid, void* buffer, uint32_t len, unsigned long _timeout) { return -1; }
int32_t SocketManager::close(uint32_t sockid) { return -1; }
    

