#include "socket.h"
#include "data_management_layer.h"
#include <cstring>

Socket::Socket() { id=-1;inUse=false; bufferLength=bufferStart=0; }

int32_t Socket::init(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif)
{
    this->family = family;
    this->type = type;
    this->protocol = protocol;
    this->port = port;
    this->nif = nif;
    return 0;
}

int32_t Socket::connect(uint32_t sockid, const sockaddr_b *addr, long addrlen)
{
    id = sockid;
    //format to tell the gateway to open a socket is: <sockid><family><type><protocol><port><address>
    uint8_t data[9+addrlen];
    data[0] = (sockid & 0xFF000000) >> 24;
    data[1] = (sockid & 0xFF0000) >> 16;
    data[2] = (sockid & 0xFF00) >> 8;
    data[3] = sockid & 0xFF;
    
    data[4] = family;
    data[5] = type;
    data[6] = protocol;
    
    data[7] = (port & 0xFF00) >> 8;
    data[8] = port & 0xFF;
    
    memcpy(data+9, addr, addrlen);
    
    DataManagementLayer::sendData(9+addrlen, data);
    return 0;
}
int32_t Socket::send(const void* data, uint32_t len)
{
    uint8_t dataPlusID[len+2];
    dataPlusID[0] = (id & 0xFF00) >> 8;
    dataPlusID[1] = id & 0xFF;
    
    memcpy(dataPlusID+2, data, len);
    DataManagementLayer::sendData(len, dataPlusID);
    return 0;
}
int32_t Socket::receive(void* data, uint32_t len, unsigned long _timeout)
{
    memcpy(data, buffer+bufferStart, len);
    bufferStart += len;
    bufferLength -= len;
    
    if (len > bufferLength) {
        //they are asking for too much data
        return -1;
    }
    
    if (bufferLength == 0) {
        bufferStart = 0;
    }
    return len;
}
int32_t Socket::close()
{
    return -1;
}

int32_t Socket::feed(void* data, uint32_t len)
{
    if (bufferLength+len > SOCKET_BUFFER_SIZE) {
        //can't put all this data in.
        return -1;
    }
    
    memcpy(buffer+bufferStart, data, len);
    bufferLength+=len;
    return 0;
}
