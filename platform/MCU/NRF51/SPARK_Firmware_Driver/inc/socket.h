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

#ifndef _SOCKET_H
#define	_SOCKET_H

#ifdef __cplusplus

#include <stdint.h>
#include "data_service.h"

#define BLUZGW_SOCKET_TIMEOUT 30 /* seconds */

enum SOCKET_COMMANDS {
    SOCKET_DATA,
    SOCKET_CONNECT,
    SOCKET_DISCONNECT,
    SOCKET_CONNECTED,
    SOCKET_FAILED
};

enum SOCKET_STATE {
  AVAILABLE,
  CONNECT_FAILED,
  CONNECTED
};

typedef struct
{
    uint16_t   sa_family;
    uint8_t    sa_data[14];
} sockaddr_b;


class Socket
{
public:

    Socket();
    
    int32_t init(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif);
    int32_t connect(uint32_t sockid, const sockaddr_b *addr, long addrlen);
    int32_t send(const void* buffer, uint32_t len);
    int32_t receive(void* buffer, uint32_t len, unsigned long _timeout);
    int32_t close();
    int32_t bytes_available();
    
    int32_t feed(uint8_t* buffer, uint32_t len);
    
    static const int32_t SOCKET_BUFFER_SIZE = 1024;
    volatile SOCKET_STATE inUse;

    uint32_t _remoteIP;

private:
    uint32_t id;
    
    uint8_t family;
    uint8_t type;
    uint8_t protocol;
    uint16_t port;
    uint32_t nif;

    uint16_t bufferLength, bufferStart;
    uint8_t buffer[SOCKET_BUFFER_SIZE];
};

#endif
#endif	/* _SOCKET_H */
