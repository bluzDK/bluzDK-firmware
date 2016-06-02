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

int X_inet_gethostbyname(const char* hostname, uint16_t hostnameLen, HAL_IPAddress* out_ip_addr,
        network_interface_t nif, void* reserved)
{
  HAL_IPAddress rip = { (uint32_t)(10<<24 | 0 <<16 | 1 << 8 | 112) };
  *out_ip_addr = rip;
  return 0;
}

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
    int rv = 0;
    if(HAL_BLE_GET_STATE() == BLUETOOTH_LE_CONNECTED)
    {
        IPAddress ip_addr;

        if((rv = X_inet_gethostbyname(host, strlen(host), ip_addr, nif, NULL)) == 0)
        {
            return connect(ip_addr, port, nif);
        }
        else {
            DEBUG("unable to get IP for hostname");
        }
  }
  return rv;
}

int TCPClient::connect(IPAddress ip, uint16_t port, network_interface_t nif)
{
    stop();
    int connected = 0;
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

        // uint32_t ot = HAL_NET_SetNetWatchDog(S2M(MAX_SEC_WAIT_CONNECT));
        DEBUG("_sock %d connect",_sock);
        connected = !(socket_connect(_sock, &tSocketAddr, sizeof(tSocketAddr))); // socket_connect() returns 0 on success
        DEBUG("_sock %d connected=%d",_sock, connected);
        //HAL_NET_SetNetWatchDog(ot);
        _remoteIP = ip;
        if(!connected)
        {
            stop();
        }
      }
    }
    return connected;
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
        return buf[0];
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
  DEBUG("_sock %d closesocket", _sock);

  if (isOpen(_sock))
      socket_close(_sock);
  _sock = socket_handle_invalid();
  _remoteIP.clear();
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
    return _remoteIP;
}
