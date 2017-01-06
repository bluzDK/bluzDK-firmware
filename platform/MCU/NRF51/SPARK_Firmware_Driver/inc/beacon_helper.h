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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define EDDYSTONE_PREFIX_COUNT 4
#define EDDYSTONE_PREFIXES (const char*[EDDYSTONE_PREFIX_COUNT]){"http://www.", "https://www.", "http://", "https://" }

#define EDDYSTONE_SUFFIX_COUNT 14
#define EDDYSTONE_SUFFIXES (const char*[EDDYSTONE_SUFFIX_COUNT]){".com/", ".org/", ".edu/", ".net/", ".info/", ".biz/", ".gov/", ".com", ".org", ".edu", ".net", ".info", ".biz", ".gov" }

typedef struct
{
    uint8_t data[18];
    uint8_t length;
    bool valid;
} eddystone_beacon_payload;

eddystone_beacon_payload parse_url(char *url);
