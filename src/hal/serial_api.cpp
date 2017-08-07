/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License.  You
 * may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied.  See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include "serial_api.h"

#include <Arduino.h>
#include <SoftwareSerial.h>

namespace mbino {

    static void serial_begin(serial_t* obj)
    {
#ifdef SERIAL_PORT_MONITOR
        if (obj->tx == 0 && obj->rx == 0) {
            SERIAL_PORT_MONITOR.begin(obj->baudrate, obj->config);
            while (!SERIAL_PORT_MONITOR)
                ;
            obj->stream = &SERIAL_PORT_MONITOR;
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE
        if (obj->tx == 1 && obj->rx == 0) {
            SERIAL_PORT_HARDWARE.begin(obj->baudrate, obj->config);
            obj->stream = &SERIAL_PORT_HARDWARE;
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE1
        if (obj->tx == 18 && obj->rx == 19) {
            SERIAL_PORT_HARDWARE1.begin(obj->baudrate, obj->config);
            obj->stream = &SERIAL_PORT_HARDWARE1;
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE2
        if (obj->tx == 16 && obj->rx == 17) {
            SERIAL_PORT_HARDWARE2.begin(obj->baudrate, obj->config);
            obj->stream = &SERIAL_PORT_HARDWARE2;
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE3
        if (obj->tx == 14 && obj->rx == 15) {
            SERIAL_PORT_HARDWARE3.begin(obj->baudrate, obj->config);
            obj->stream = &SERIAL_PORT_HARDWARE3;
            return;
        }
#endif
        SoftwareSerial* serial = new SoftwareSerial(obj->rx, obj->tx);
        serial->begin(obj->baudrate);  // config not supported!
        obj->stream = serial;
    }

    void serial_init(serial_t* obj, PinName tx, PinName rx)
    {
        obj->stream = 0;
        obj->tx = tx;
        obj->rx = rx;
        obj->baudrate = 9600;
        obj->config = SERIAL_8N1;
    }

#ifdef SERIAL_PORT_MONITOR
    void serial_usb_init(serial_t* obj)
    {
        serial_init(obj, 0, 0);
    }
#endif

    void serial_free(serial_t* obj)
    {
#ifdef SERIAL_PORT_MONITOR
        if (obj->stream == &SERIAL_PORT_MONITOR) {
            SERIAL_PORT_MONITOR.end();
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE
        if (obj->stream == &SERIAL_PORT_HARDWARE) {
            SERIAL_PORT_HARDWARE.end();
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE1
        if (obj->stream == &SERIAL_PORT_HARDWARE1) {
            SERIAL_PORT_HARDWARE1.end();
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE2
        if (obj->stream == &SERIAL_PORT_HARDWARE2) {
            SERIAL_PORT_HARDWARE2.end();
            return;
        }
#endif
#ifdef SERIAL_PORT_HARDWARE3
        if (obj->stream == &SERIAL_PORT_HARDWARE3) {
            SERIAL_PORT_HARDWARE3.end();
            return;
        }
#endif
        // TODO: test w/deleter function set in serial_init()
        // no virtual Stream destructor...
        delete static_cast<SoftwareSerial*>(obj->stream);
    }

    void serial_baud(serial_t* obj, long baudrate)
    {
        // FIXME: change baudrate on active stream
        obj->baudrate = baudrate;
    }

    void serial_format(serial_t* obj, uint8_t data_bits, SerialParity parity, uint8_t stop_bits)
    {
        // FIXME: change config on active stream
        obj->config = parity << 4 | (((stop_bits - 1) & 0x01) << 3) | (((data_bits - 5) & 0x03) << 1);
    }

    int serial_getc(serial_t* obj)
    {
        if (!obj->stream) {
            serial_begin(obj);
        }
        return obj->stream->read();
    }

    void serial_putc(serial_t* obj, int c)
    {
        if (!obj->stream) {
            serial_begin(obj);
        }
        obj->stream->write(c);
    }

    void serial_puts(serial_t* obj, const char* s)
    {
        if (!obj->stream) {
            serial_begin(obj);
        }
        obj->stream->write(s);
    }

    bool serial_readable(serial_t* obj)
    {
        if (!obj->stream) {
            serial_begin(obj);
        }
        return obj->stream->available() != 0;
    }

    bool serial_writable(serial_t* obj)
    {
        if (!obj->stream) {
            serial_begin(obj);
        }
        return obj->stream->availableForWrite() != 0;
    }

}
