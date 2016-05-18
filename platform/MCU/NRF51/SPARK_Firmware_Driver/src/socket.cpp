/**
 Copyright (c) 2015 MidAir Technology, LLC.  All rights reserved.
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation, either
 version 3 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "socket.h"
#include "data_management_layer.h"
#include "debug.h"
#include "registered_data_services.h"
#include <cstring>
#include <stdio.h>

Socket::Socket() { id=-1;inUse=false; bufferLength=0;bufferStart=0; }

int32_t Socket::init(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif)
{
    bufferLength=0;
    bufferStart=0;
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
    int offset = 0;
#if PLATFORM_ID==269
    offset = 3;
#endif

    id = sockid;

    //format to tell the gateway to open a socket is: <sockid><family><type><protocol><port><address>
    uint8_t data[9+addrlen+offset];
    //first, the service ID
    data[0+offset] = SOCKET_DATA_SERVICE & 0xFF;
    
    //next, the socket ID
    data[1+offset] = ((SOCKET_CONNECT << 4) & 0xF0) | (id & 0x0F);
    
    //then the connection code
    data[2+offset] = (SOCKET_CONNECT & 0xFF00) >> 8;
    data[3+offset] = SOCKET_CONNECT & 0xFF;
    
    data[4+offset] = family;
    data[5+offset] = type;
    data[6+offset] = protocol;

    data[7+offset] = (port & 0xFF00) >> 8;
    data[8+offset] = port & 0xFF;
    
    memcpy(data+9+offset, addr, addrlen);
    
    DataManagementLayer::sendData(9+addrlen+offset, data);
    return 0;
}
int32_t Socket::send(const void* data, uint32_t len)
{
    int offset = 0;
#if PLATFORM_ID==269
    offset = 3;
#endif

    DEBUG("Sending on socket %d data of size %d!", id, len);
    uint8_t dataPlusID[len+2+offset];
    dataPlusID[0+offset] = SOCKET_DATA_SERVICE & 0xFF;
    dataPlusID[1+offset] = ((SOCKET_DATA << 4) & 0xF0) | (id & 0x0F);
    
    memcpy(dataPlusID+2+offset, data, len);
    
//    uint8_t dataPlusID[len];
//    memcpy(dataPlusID, data, len);
    
    DataManagementLayer::sendData(len+2+offset, dataPlusID);
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
    
    memcpy(data, buffer+bufferStart, bytesToCopy);
    bufferStart += bytesToCopy;
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
    bufferLength=0;
    bufferStart=0;
    inUse = false;
//    uint8_t data[2];
//    data[0] = SOCKET_DATA_SERVICE & 0xFF;
//    data[1] = (SOCKET_DISCONNECT & 0xF0) | (id & 0x0F);
//    
//    DataManagementLayer::sendData(2, data);
    return 0;
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
