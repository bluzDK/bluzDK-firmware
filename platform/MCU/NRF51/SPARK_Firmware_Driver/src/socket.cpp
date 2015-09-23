#include "socket.h"
#include "data_management_layer.h"
#include "debug.h"
#include "registered_data_services.h"
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
    uint8_t data[11+addrlen];
    //first, the service ID
    data[0] = (SOCKET_DATA_SERVICE & 0xFF00) >> 8;
    data[1] = SOCKET_DATA_SERVICE & 0xFF;
    
    //next, the socket ID
    data[2] = (sockid & 0xFF00) >> 8;
    data[3] = sockid & 0xFF;
    
    //then the connection code
    data[4] = (SOCKET_CONNECT & 0xFF00) >> 8;
    data[5] = SOCKET_CONNECT & 0xFF;
    
    data[6] = family;
    data[7] = type;
    data[8] = protocol;
    
    data[9] = (port & 0xFF00) >> 8;
    data[10] = port & 0xFF;
    
    memcpy(data+11, addr, addrlen);
    
    DataManagementLayer::sendData(11+addrlen, data);
    return 0;
}
int32_t Socket::send(const void* data, uint32_t len)
{
    DEBUG("Sending on socket %d data of size %d!", id, len);
    uint8_t dataPlusID[len+4];
    dataPlusID[0] = (SOCKET_DATA_SERVICE & 0xFF00) >> 8;
    dataPlusID[1] = SOCKET_DATA_SERVICE & 0xFF;
    dataPlusID[2] = (id & 0xFF00) >> 8;
    dataPlusID[3] = id & 0xFF;
    
    memcpy(dataPlusID+4, data, len);
    
//    uint8_t dataPlusID[len];
//    memcpy(dataPlusID, data, len);
    
    DataManagementLayer::sendData(len+4, dataPlusID);
    return len;
}
int32_t Socket::receive(void* data, uint32_t len, unsigned long _timeout)
{
    if (bufferLength > 0) {
        DEBUG("When entering receive, buffer length is %d and buffer start is %d", bufferLength, bufferStart);
    }
    
    int bytesToCopy = len;
    if (len > bufferLength) {
        //they are asking for too much data
        bytesToCopy = bufferLength;
    }
    
    int startTemp = bufferStart;
    bufferStart += bytesToCopy;
    memcpy(data, buffer+startTemp, bytesToCopy);
    bufferLength -= bytesToCopy;
    
    if (bufferLength == 0) {
        bufferStart = 0;
    }
    
    
    if (bytesToCopy > 0) {
        DEBUG("Socket data was received from id %d, read %d bytes. Leaving us %d bytes left, starting at %d", id, bytesToCopy, bufferLength, bufferStart);
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
    
    memcpy(buffer+bufferStart+bufferLength, data, len);
    bufferLength+=len;
//    DEBUG("Fed %d bytes to socket id %d, leaving it with %d bytes", len, id, bufferLength);
    
    return 0;
}
