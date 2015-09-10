#include "socket.h"

Socket::Socket() { inUse = false; }

int32_t Socket::connect(uint32_t sockid, const sockaddr_b *addr, long addrlen) { return -1; }
int32_t Socket::send(const void* buffer, uint32_t len) { return -1; }
int32_t Socket::receive(void* buffer, uint32_t len, unsigned long _timeout) { return -1; }
int32_t Socket::close() { return -1; }
