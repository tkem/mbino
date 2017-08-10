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

    template<class T>
    void serial_stream_begin(serial_stream_t* obj, long baud, uint8_t config)
    {
        T* stream = static_cast<T*>(obj);
        stream->begin(baud, config);
        // wait for serial port to become ready
        while (!*stream)
            ;
    }

    template<class T>
    void serial_stream_end(serial_stream_t* obj)
    {
        static_cast<T*>(obj)->end();
    }

    template<>
    void serial_stream_begin<SoftwareSerial>(serial_stream_t* obj, long baud, uint8_t)
    {
        // config not supported
        static_cast<SoftwareSerial*>(obj)->begin(baud);
    }

    template<>
    void serial_stream_end<SoftwareSerial>(serial_stream_t* obj)
    {
        // no virtual destructor
        delete static_cast<SoftwareSerial*>(obj);
    }

    template<class T>
    static void serial_init(serial_t* obj, T* stream)
    {
        static const serial_stream_interface_t interface = {
            &serial_stream_begin<T>,
            &serial_stream_end<T>
        };
        obj->interface = &interface;
        obj->stream = stream;
        obj->baudrate = 9600;
        obj->config = SERIAL_8N1;
        obj->initialized = false;
    }

    static void serial_begin(serial_t* obj)
    {
        // serial API is not synchronized
        if (!obj->initialized) {
            obj->interface->begin(obj->stream, obj->baudrate, obj->config);
            obj->initialized = true;
        }
    }

    static void serial_restart(serial_t* obj)
    {
        // serial API is not synchronized
        if (obj->initialized) {
            obj->stream->flush();
            obj->interface->begin(obj->stream, obj->baudrate, obj->config);
        }
    }

    void serial_init(serial_t* obj, PinName tx, PinName rx)
    {
        if (tx == NC && rx == NC) {
            // FIXME: not supported?
        }
#ifdef SERIAL_PORT_HARDWARE
        else if (tx == 1 && rx == 0) {
            serial_init(obj, &SERIAL_PORT_HARDWARE);
        }
#endif
#ifdef SERIAL_PORT_HARDWARE1
        else if (tx == 19 && rx == 18) {
            serial_init(obj, &SERIAL_PORT_HARDWARE1);
        }
#endif
#ifdef SERIAL_PORT_HARDWARE2
        else if (tx == 16 && rx == 17) {
            serial_init(obj, &SERIAL_PORT_HARDWARE2);
        }
#endif
#ifdef SERIAL_PORT_HARDWARE3
        else if (tx == 14 && rx == 15) {
            serial_init(obj, &SERIAL_PORT_HARDWARE3);
        }
#endif
        else {
            serial_init(obj, new SoftwareSerial(rx, tx));
        }
    }

    // TODO: compile-time error?
#ifdef SERIAL_PORT_MONITOR
    void serial_usb_init(serial_t* obj)
    {
        serial_init(obj, &SERIAL_PORT_MONITOR);
    }
#endif

    void serial_free(serial_t* obj)
    {
        obj->interface->end(obj->stream);
    }

    void serial_baud(serial_t* obj, long baudrate)
    {
        obj->baudrate = baudrate;
        serial_restart(obj);
    }

    void serial_format(serial_t* obj, uint8_t data_bits, SerialParity parity, uint8_t stop_bits)
    {
        uint8_t config = (((data_bits - 5) & 0x3) << 1) | (((stop_bits - 1) & 0x1) << 3);
        switch (parity) {
        case ParityNone:
            config |= SERIAL_8N1 & 0xF0;
            break;
        case ParityOdd:
            config |= SERIAL_8O1 & 0xF0;
            break;
        case ParityEven:
            config |= SERIAL_8E1 & 0xF0;
            break;
        }
        obj->config = config;
        serial_restart(obj);
    }

    int serial_getc(serial_t* obj)
    {
        serial_begin(obj);
        return obj->stream->read();
    }

    void serial_putc(serial_t* obj, int c)
    {
        serial_begin(obj);
        obj->stream->write(c);
    }

    void serial_puts(serial_t* obj, const char* s)
    {
        serial_begin(obj);
        obj->stream->write(s);
    }

    bool serial_readable(serial_t* obj)
    {
        return obj->stream->available() != 0;
    }

    bool serial_writable(serial_t* obj)
    {
        return obj->stream->availableForWrite() != 0;
    }

}
