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

#include "socket_hal.h"
#include "socket_manager.h"


int32_t socket_connect(sock_handle_t sd, const sockaddr_t *addr, long addrlen)
{
    return SocketManager::instance()->connect(sd, (const sockaddr_b*)addr, addrlen);
}

sock_result_t socket_reset_blocking_call()
{
    return 0;
}

sock_result_t socket_receive(sock_handle_t sd, void* buffer, socklen_t len, system_tick_t _timeout)
{
  return SocketManager::instance()->receive(sd, buffer, len, _timeout);
}

sock_result_t socket_create_nonblocking_server(sock_handle_t sock, uint16_t port)
{
    return 0;
}

sock_result_t socket_receivefrom(sock_handle_t sock, void* buffer, socklen_t bufLen, uint32_t flags, sockaddr_t* addr, socklen_t* addrsize)
{
    return 0;
}

sock_result_t socket_bind(sock_handle_t sock, uint16_t port)
{
    return 0;
}

sock_result_t socket_accept(sock_handle_t sock)
{
    return 0;
}

uint8_t socket_active_status(sock_handle_t socket)
{
    return SocketManager::instance()->active_status(socket);
}

sock_result_t socket_close(sock_handle_t sock)
{
    return SocketManager::instance()->close(sock);
}

sock_handle_t socket_create(uint8_t family, uint8_t type, uint8_t protocol, uint16_t port, network_interface_t nif)
{
 return SocketManager::instance()->create(family, type, protocol, port, nif);
}

sock_result_t socket_send(sock_handle_t sd, const void* buffer, socklen_t len)
{
    return SocketManager::instance()->send(sd, buffer, len);
}

sock_result_t socket_sendto(sock_handle_t sd, const void* buffer, socklen_t len, uint32_t flags, sockaddr_t* addr, socklen_t addr_size)
{
    return 0;
}

sock_result_t socket_bytes_available(sock_handle_t sd) {
    return SocketManager::instance()->bytes_available(sd);
}

const sock_handle_t SOCKET_INVALID = (sock_handle_t)-1;

sock_handle_t socket_handle_invalid()
{
    return SOCKET_INVALID;
}

uint8_t socket_handle_valid(sock_handle_t handle)
{
    return handle != SOCKET_INVALID;
}

sock_result_t socket_create_tcp_server(uint16_t port, network_interface_t nif)
{
    return SOCKET_INVALID;
}

sock_result_t socket_join_multicast(const HAL_IPAddress* addr, network_interface_t nif, void* reserved)
{
    /* Not supported on bluz */
    return -1;
}

sock_result_t socket_leave_multicast(const HAL_IPAddress* addr, network_interface_t nif, void* reserved)
{
    /* Not supported on bluz */
    return -1;
}

sock_result_t socket_peer(sock_handle_t sd, sock_peer_t* peer, void* reserved)
{
    return -1;
}

uint32_t socket_remoteIP(sock_handle_t sd) {
  return SocketManager::instance()->remoteIP(sd);
}

