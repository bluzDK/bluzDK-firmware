#include "socket.h"
#include "data_management_layer.h"
#include "debug.h"
#include <cstring>
#include <stdio.h>

Socket::Socket() { id=-1;inUse=false; bufferLength=0;bufferStart=0; }

int32_t Socket::init(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif)
{
    inUse = true;
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
    DEBUG("Sending on socket %d data of size %d!", id, len);
//    uint8_t dataPlusID[len+2];
//    dataPlusID[0] = (id & 0xFF00) >> 8;
//    dataPlusID[1] = id & 0xFF;
//    
//    memcpy(dataPlusID+2, data, len);
    
    uint8_t dataPlusID[len];
    memcpy(dataPlusID, data, len);
    
    DataManagementLayer::sendData(len, dataPlusID);
    return len;
}
int32_t Socket::receive(void* data, uint32_t len, unsigned long _timeout)
{
    int bytesToCopy = len;
    if (len > bufferLength) {
        //they are asking for too much data
        bytesToCopy = bufferLength;
    }
    
    memcpy(data, buffer+bufferStart, bytesToCopy);
    
    bufferStart += bytesToCopy;
    bufferLength -= bytesToCopy;
    
    if (bufferLength == 0) {
        bufferStart = 0;
    }
    
    
    if (bytesToCopy > 0) {
        DEBUG("Socket data was received from id %d, read %d bytes. Leaving us %d bytes left", id, bytesToCopy, bufferLength);
    }
    
    return bytesToCopy;
}
int32_t Socket::close()
{
    inUse = false;
    return 1;
}

int32_t Socket::bytes_available()
{
    return bufferLength;
}

int32_t Socket::feed(uint8_t* data, uint32_t len)
{
    if (bufferLength+len > SOCKET_BUFFER_SIZE) {
        //can't put all this data in.
        return -1;
    }
    
    memcpy(buffer+bufferStart, data, len);
    bufferLength+=len;
    DEBUG("Fed %d bytes to socket id %d, leaving it with %d bytes", len, id, bufferLength);
    
    return 0;
}
