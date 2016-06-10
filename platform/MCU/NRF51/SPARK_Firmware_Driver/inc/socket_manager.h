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

#ifndef _SOCKET_MANAGER_H
#define	_SOCKET_MANAGER_H

#ifdef __cplusplus

#include <stdint.h>
#include <stdlib.h>
#include "socket.h"

static const uint8_t SOCKET_INACTIVE = 1;
static const uint8_t SOCKET_ACTIVE = 0;

class SocketManager : public DataService
{
public:
    static SocketManager* instance();
    
    int32_t create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif);
    int32_t connect(uint32_t sd, const sockaddr_b *addr, long addrlen);
    int32_t send(uint32_t sockid, const void* buffer, uint32_t len);
    int32_t receive(uint32_t sockid, void* buffer, uint32_t len, unsigned long _timeout);
    int32_t active_status(uint32_t sockid);
    int32_t close(uint32_t sockid);
    int32_t bytes_available(uint32_t sockid);
    uint32_t remoteIP(uint32_t sockid);
    
    static const int32_t MAX_NUMBER_OF_SOCKETS = 2;
    
    //DataService functions
    virtual int32_t getServiceID();
    virtual int32_t DataCallback(uint8_t *data, int16_t length);
    
private:
    Socket sockets[MAX_NUMBER_OF_SOCKETS];

    //this is a singleton class, so these all need to be private so they can't be called
    SocketManager(){};
    SocketManager(SocketManager const&){};
    SocketManager& operator=(SocketManager const&);
    static SocketManager* m_pInstance;
};

#endif
#endif	/* _SOCKET_MANAGER_H */

