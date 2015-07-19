#include <stdint.h>
#include "stream_manager.h"

StreamManager::StreamManager() { }
    
int32_t StreamManager::connect(const sockaddr_b *addr, long addrlen) { return 0; }

int32_t StreamManager::send(uint32_t sockid, const void* buffer, uint32_t len) { return 0; }

int32_t StreamManager::receive(uint32_t sockid, void* buffer, uint32_t len, unsigned long _timeout) { return 0; }

int32_t StreamManager::close(uint32_t sockid) { return 0; }