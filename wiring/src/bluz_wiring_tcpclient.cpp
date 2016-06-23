/**
 ******************************************************************************
 * @file    spark_wiring_tcpclient.cpp
 * @author  Bryan J. Rentoul (aka Gruvin)
 * @version V1.0.0
 * @date    10-Nov-2013
 *
 * Updated: 14-Feb-2014 David Sidrane <david_s5@usa.net>
 * @brief
 ******************************************************************************
  Copyright (c) 2013-2015 Particle Industries, Inc.  All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, see <http://www.gnu.org/licenses/>.
  ******************************************************************************
 */

#include "bluz_wiring_tcpclient.h"
#include "bluetooth_le_hal.h"
#include "socket_hal.h"

uint16_t TCPClient::_srcport = 1024;

static bool inline isOpen(sock_handle_t sd)
{
   return socket_handle_valid(sd);
}

TCPClient::TCPClient() : TCPClient(socket_handle_invalid())
{
}

TCPClient::TCPClient(sock_handle_t sock) : _sock(sock)
{
}

int TCPClient::connect(const char* host, uint16_t port, network_interface_t nif)
{
    stop();
    int result = 0;
    if(HAL_BLE_GET_STATE() == BLUETOOTH_LE_CONNECTED)
    {
      // stack the domain name over top of sockaddr_t's IP address data
      union {
        sockaddr_t tSocketAddr;
        struct {
          uint8_t filler[sizeof(tSocketAddr.sa_family) + sizeof(uint16_t/* tcp_port*/)];
          char domain[256];
        } host;
      } destAddr;

      _sock = socket_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, port, nif);
      DEBUG("socket=%d",_sock);

      if (socket_handle_valid(_sock))
      {

        destAddr.tSocketAddr.sa_family = AF_INET;

        destAddr.tSocketAddr.sa_data[0] = (port & 0xFF00) >> 8;
        destAddr.tSocketAddr.sa_data[1] = (port & 0x00FF);

        strcpy(destAddr.host.domain, host); // includes null terminator

        DEBUG("_sock %d connect",_sock);

        // socket_connect() returns 0 on success
        result = !(socket_connect(_sock, (/* ugly hack*/sockaddr_t *)&destAddr, sizeof(destAddr.host.filler) + strlen(destAddr.host.domain)+1));
        
        DEBUG("_sock %d connected=%d",_sock, result);
        
        if(!result)
        {
            stop();
        }
      }
    }
    return result;
}

int TCPClient::connect(IPAddress ip, uint16_t port, network_interface_t nif)
{
    stop();
    int result = 0;
    if(HAL_BLE_GET_STATE() == BLUETOOTH_LE_CONNECTED)
    {
      sockaddr_t tSocketAddr;
      _sock = socket_create(AF_INET, SOCK_STREAM, IPPROTO_TCP, port, nif);
      DEBUG("socket=%d",_sock);

      if (socket_handle_valid(_sock))
      {

        tSocketAddr.sa_family = AF_INET;

        tSocketAddr.sa_data[0] = (port & 0xFF00) >> 8;
        tSocketAddr.sa_data[1] = (port & 0x00FF);

        tSocketAddr.sa_data[2] = ip[0];        // Todo IPv6
        tSocketAddr.sa_data[3] = ip[1];
        tSocketAddr.sa_data[4] = ip[2];
        tSocketAddr.sa_data[5] = ip[3];

        DEBUG("_sock %d connect",_sock);
        result = !(socket_connect(_sock, &tSocketAddr, sizeof(tSocketAddr))); // socket_connect() returns 0 on success
        DEBUG("_sock %d connected=%d",_sock, result);
        if(!result)
        {
            stop();
        }
      }
    }
    return result;
}

size_t TCPClient::write(uint8_t b)
{
    return write(&b, 1);
}

size_t TCPClient::write(const uint8_t *buffer, size_t size)
{
    return status() ? socket_send(_sock, buffer, size) : -1;
}

int TCPClient::available()
{
    return socket_bytes_available(_sock);
}

int TCPClient::read()
{
    uint8_t buf[1];
    if (read(buf, 1) == 1) {
        return (int)buf[0];
    } else {
        return -1;
    }
}

int TCPClient::read(uint8_t *buffer, size_t size)
{
    return socket_receive(_sock, buffer , size, 0);
}

int TCPClient::peek()
{
    //TO DO: Implement this
    return  -1;
}

void TCPClient::flush()
{
  while (available())
    read();
}


void TCPClient::stop()
{
  DEBUG("_sock %d close socket", _sock);
  if (isOpen(_sock))
      socket_close(_sock);
  _sock = socket_handle_invalid();
}

uint8_t TCPClient::connected()
{
  return socket_active_status(_sock) == SOCKET_STATUS_ACTIVE;
}

uint8_t TCPClient::status()
{
  return (isOpen(_sock) && HAL_BLE_GET_STATE() == BLUETOOTH_LE_CONNECTED && (SOCKET_STATUS_ACTIVE == socket_active_status(_sock)));
}

TCPClient::operator bool()
{
  return (status()!=0);
}

IPAddress TCPClient::remoteIP()
{
  IPAddress ip(socket_remoteIP(_sock));
  return ip;
}
