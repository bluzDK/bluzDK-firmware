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

#include "beacon_helper.h"

eddystone_beacon_payload parse_url(char *url)
{
    eddystone_beacon_payload packet;
    packet.length = 0;

    uint8_t prefix = 0xff;
    uint8_t prefix_length = 0;

    for (int i = 0; i < EDDYSTONE_PREFIX_COUNT; i++) {
        if (strncmp(EDDYSTONE_PREFIXES[i], url, strlen(EDDYSTONE_PREFIXES[i])) == 0) {
            prefix = i;
            break;
        }
    }

    //if there is a prefix, calculate the length of the packet we need
    uint16_t length = strlen(url);
    if (prefix != 0xff) {
        packet.data[0] = prefix;
        prefix_length = strlen(EDDYSTONE_PREFIXES[prefix]);
        length = length - strlen(EDDYSTONE_PREFIXES[prefix]) + 1;
        packet.length++;
    }
    memcpy(packet.data+packet.length, url+prefix_length, length);
    packet.length = length;

    return packet;
}