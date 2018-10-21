/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
#ifdef ARDUINO_ARCH_SAMD

#include <Arduino.h>

#include "platform/mbed_retarget.h"
#include "platform/mbino_error.h"

void initVariant() __attribute__((weak));
void initVariant() { }

extern bool serial_port_monitor_initialized;

static bool initialized()
{
    // TODO
    return true;
}

void mbino_serial_port_monitor_init()
{
#ifdef SERIAL_PORT_MONITOR
    if (!serial_port_monitor_initialized) {
        // in Arduino main.cpp, __libc_init_array() is called after init()
        if (!initialized()) {
            initVariant();
            delay(1);
#ifdef USBCON
            USBDevice.init();
            USBDevice.attach();
#endif
        }
        SERIAL_PORT_MONITOR.begin(9600);
#ifdef USBCON
        // wait for a limited time for port to become ready
        for (unsigned n = 50000; !SERIAL_PORT_MONITOR && n; --n)
            ;
#endif
    }
#endif
}

mbed::FileHandle* mbed::mbed_target_override_console(int fd)
{
    return NULL;  // TODO
}

MBED_WEAK ::Stream* mbino_target_override_monitor()
{
    return NULL;  // TODO
}

#endif
