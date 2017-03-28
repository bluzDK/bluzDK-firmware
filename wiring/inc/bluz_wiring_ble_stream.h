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

#ifndef __BLUZ_WIRING_BLE_STREAM_H
#define __BLUZ_WIRING_BLE_STREAM_H

#include "spark_wiring_stream.h"
#include "ble_stream_hal.h"

class BLEStream : public Stream
{
public:
    BLEStream();
    virtual ~BLEStream() {};
    void begin();
    void end();

    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    size_t write(uint16_t);
    virtual size_t write(uint8_t);

    inline size_t write(unsigned long n) { return write((uint16_t)n); }
    inline size_t write(long n) { return write((uint16_t)n); }
    inline size_t write(unsigned int n) { return write((uint16_t)n); }
    inline size_t write(int n) { return write((uint16_t)n); }

    using Print::write; // pull in write(str) and write(buf, size) from Print

    operator bool();

    bool isEnabled(void);
};

#define BLESerial __fetch_global_bleserial()
extern BLEStream& __fetch_global_bleserial();

#endif /* __BLUZ_WIRING_BLE_STREAM_H */