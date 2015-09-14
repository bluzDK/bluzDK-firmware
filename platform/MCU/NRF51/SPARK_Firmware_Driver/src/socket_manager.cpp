
#include "socket_manager.h"
#include "registered_data_services.h"

SocketManager* SocketManager::m_pInstance = NULL;

SocketManager* SocketManager::instance()
{
    if (!m_pInstance)   // Only allow one instance of class to be generated.
        m_pInstance = new SocketManager;
    return m_pInstance;

}

int32_t SocketManager::create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, uint32_t nif)
{
    for (int i = 0; i < MAX_NUMBER_OF_SOCKETS; i++)
    {
        if (!sockets[i].inUse)
        {
            sockets[i].inUse = true;
            sockets[i].init(family, type, protocol, port, nif);
            return i;
        }
    }
    return -1;
}
int32_t SocketManager::connect(uint32_t sd, const sockaddr_b *addr, long addrlen)
{
    return sockets[sd].connect(sd, addr, addrlen);
}
int32_t SocketManager::send(uint32_t sd, const void* buffer, uint32_t len)
{
    return sockets[sd].send(buffer, len);
}
int32_t SocketManager::receive(uint32_t sd, void* buffer, uint32_t len, unsigned long _timeout)
{
    return sockets[sd].receive(buffer, len, _timeout);
}
int32_t SocketManager::close(uint32_t sockid)
{
    sockets[sockid].inUse = false;
    return sockets[sockid].close();
}


//DataService functions
int32_t SocketManager::getServiceID()
{
    return SOCKET_DATA_SERVICE;
}
int32_t SocketManager::DataCallback(uint8_t *data, int16_t length)
{
    uint16_t socketID = (data[0] << 8) | data[1];
    if (sockets[socketID].inUse)
    {
        sockets[socketID].feed(data, length-2);
    }
    return -1;
}

