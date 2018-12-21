/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2018 Thomas Kemmer
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
#ifdef ARDUINO_ARCH_AVR

#include <Arduino.h>

#include "platform/platform.h"
#include "platform/FileHandle.h"
#include "platform/mbed_retarget.h"

// define here, or this will pull in Arduino's main.cpp
void initVariant() __attribute__((weak));
void initVariant() { }

extern "C" void mbed_sdk_init()
{
    static bool mbed_sdk_inited = false;

    if (!mbed_sdk_inited) {
        // if the application doesn't define its own main() this will
        // be run twice, setting UCSRB/UCSR0B = 0 before setup()
        init();
        initVariant();
#if defined(USBCON)
	USBDevice.attach();
#endif
        mbed_sdk_inited = true;
    }
}

template<class T>
mbed::FileHandle* get_console()
{
    return NULL;
}

#ifdef SERIAL_PORT_USBVIRTUAL
// SERIAL_PORT_USBVIRTUAL implementations may differ between hardware
// platforms, so this belongs in target
class USBVirtualSerial : public mbed::FileHandle {
public:
    USBVirtualSerial() {
        SERIAL_PORT_USBVIRTUAL.begin(0);
    }

    ~USBVirtualSerial() {
#if MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT
        if (connected()) {
            SERIAL_PORT_USBVIRTUAL.flush();
        }
#endif
    }

    ssize_t write(const void* buffer, size_t size) {
        if (connected()) {
             size_t n = SERIAL_PORT_USBVIRTUAL.write(static_cast<const uint8_t*>(buffer), size);
#if !MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL
             SERIAL_PORT_USBVIRTUAL.flush();
#endif
            return n;
        } else {
            return 0;
        }
    }

    ssize_t read(void* buffer, size_t size) {
        if (size != 0 && connected()) {
            size_t n;
            do {
                n = SERIAL_PORT_USBVIRTUAL.readBytes(static_cast<uint8_t*>(buffer), size);
            } while (n == 0);
            return n;
        } else {
            return 0;
        }
    }

    off_t seek(off_t offset, int whence = SEEK_SET) {
        return -ESPIPE;
    }

    off_t size() {
        return -EINVAL;
    }

    int isatty() {
        return true;
    }

    int close() {
        return 0;
    }

    int sync() {
        SERIAL_PORT_USBVIRTUAL.flush();
        return 0;
    }

    short poll(short events) const {
        short revents = 0;
        if ((events & POLLIN) && SERIAL_PORT_USBVIRTUAL.available()) {
            revents |= POLLIN;
        }
        if ((events & POLLOUT) && SERIAL_PORT_USBVIRTUAL.availableForWrite()) {
            revents |= POLLOUT;
        }
        return revents;
    }

private:
    static bool connected() {
        // On the Leonardo, Serial_::operator bool() calls delay(10),
        // so we should not check for every read/write...
        static bool inited = false;
        static bool ready = false;
        if (!inited) {
            ready = connect();
            inited = true;
        }
        return ready;
    }

    static bool connect() {
        uint8_t sreg = SREG;
        sei();
        for (int n = 300; n; --n) {
            // this must be called with interrupts enabled
            if (SERIAL_PORT_USBVIRTUAL) {
                SREG = sreg;
                return true;
            }
        }
        SREG = sreg;
        return false;
    }
};

template<>
mbed::FileHandle* get_console<decltype(SERIAL_PORT_USBVIRTUAL)>()
{
    static USBVirtualSerial console;
    return &console;
}
#endif

mbed::FileHandle* mbed::mbed_target_override_console(int fd)
{
#ifdef SERIAL_PORT_MONITOR
    return get_console<decltype(SERIAL_PORT_MONITOR)>();
#else
    return NULL;
#endif
}

#endif
