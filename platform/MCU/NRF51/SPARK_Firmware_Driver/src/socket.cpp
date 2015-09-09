#include "socket.h"

Socket::Socket() { inUse = false; }

int32_t Socket::connect(const sockaddr_b *addr, long addrlen) { return -1; }
int32_t Socket::send(uint32_t sockid, const void* buffer, uint32_t len) { return -1; }
int32_t Socket::receive(uint32_t sockid, void* buffer, uint32_t len, unsigned long _timeout) { return -1; }
int32_t Socket::close(uint32_t sockid) { return -1; }
