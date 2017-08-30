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
#ifndef MBINO_SERIAL_API_H
#define MBINO_SERIAL_API_H

#include "platform/platform.h"

#ifdef DEVICE_SERIAL

class Stream; // forward declaration of Arduino Stream type

// TBD: extern "C"?
namespace mbino {

    typedef enum {
        ParityNone = 0,
        ParityOdd = 1,
        ParityEven = 2
    } SerialParity;

    typedef Stream serial_stream_t;

    struct serial_stream_interface_t;

    struct serial_t {
        const serial_stream_interface_t* interface;
        serial_stream_t* stream;
        long baudrate;
        uint8_t config;
        bool initialized;
    };

    void serial_init(serial_t* obj, PinName tx, PinName rx);

    void serial_monitor_init(serial_t* obj);

    void serial_free(serial_t* obj);

    void serial_baud(serial_t* obj, long baudrate);

    void serial_format(serial_t* obj, uint8_t data_bits, SerialParity parity, uint8_t stop_bits);

    int serial_getc(serial_t* obj);

    void serial_putc(serial_t* obj, int c);

    void serial_puts(serial_t* obj, const char* s);

    bool serial_readable(serial_t* obj);

    bool serial_writable(serial_t* obj);

}

#endif

#endif
