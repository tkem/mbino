/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
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
#ifndef MBINO_SERIAL_STREAM_OBJECT_H
#define MBINO_SERIAL_STREAM_OBJECT_H

#include <Arduino.h>

#if defined(SERIAL_PORT_MONITOR) || defined(SERIAL_PORT_HARDWARE)

struct Stream; // forward declaration of Arduino C++ Stream class

struct serial_stream_interface_s {
    void (*begin)(struct Stream *obj, long baud, uint8_t cs, uint8_t pm, uint8_t sb);
    void (*end)(struct Stream *obj);
};

#ifdef __cplusplus

template<class T>
struct serial_stream_interface {
    static const struct serial_stream_interface_s interface;
};

template<class T>
const struct serial_stream_interface_s serial_stream_interface<T>::interface = {
    &T::begin,
    &T::end
};

template<class T>
struct serial_stream : public serial_stream_interface<serial_stream<T>> {

    static void begin(Stream* obj, long baud, uint8_t cs, uint8_t pm, uint8_t sb) {
        static_cast<T*>(obj)->begin(baud, config(cs, pm, sb));
    }

    static void end(Stream* obj) {
        static_cast<T*>(obj)->end();
    }

    static uint8_t config(uint8_t cs, uint8_t pm, uint8_t sb) {
#if defined(UCSZ00) && defined(UPM00) && defined(USBS0)
        return (cs << UCSZ00) | (((0x4 - pm) & 0x3) << UPM00) | (sb << USBS0);
#elif defined(UCSZ10) && defined(UPM10) && defined(USBS1)
        return (cs << UCSZ10) | (((0x4 - pm) & 0x3) << UPM10) | (sb << USBS1);
#else
#error "Unknown UCSRC"
#endif
    }

};

#ifdef SERIAL_PORT_USBVIRTUAL

template<>
struct serial_stream<decltype(SERIAL_PORT_USBVIRTUAL)> :
    public serial_stream_interface<serial_stream<decltype(SERIAL_PORT_USBVIRTUAL)>>
{

    static void begin(Stream* obj, long baud, uint8_t, uint8_t, uint8_t) {
        auto stream = static_cast<decltype(SERIAL_PORT_USBVIRTUAL)*>(obj);
        stream->begin(baud);
        // poll stream every 10ms for max. 1 second
        for (int n = 100; !*stream && n != 0; --n) {
            delayMicroseconds(10000);  // works even before main()/init()
        }
    }

    static void end(Stream* obj) {
        static_cast<decltype(SERIAL_PORT_USBVIRTUAL)*>(obj)->end();
    }

};

#endif

#endif

#endif

#endif
