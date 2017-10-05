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
#ifdef ARDUINO_ARCH_AVR

#include "hal/gpio_api.h"
#include "hal/serial_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_wait_api.h"

#include <Arduino.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#define ERROR_BUF_SIZE 128

#if DEVICE_SERIAL
extern bool serial_port_monitor_initialized;
#endif

static void delay_ms(int ms)
{
    // delayMicroseconds() is usable before setup()
    for (int i = 0; i != ms ; ++i) {
        delayMicroseconds(1000);
    }
}

void mbed_mac_address(char *mac)
{
    mac[0] = 0x00;
    mac[1] = 0x02;
    mac[2] = 0xF7;
    mac[3] = 0xF0;
    mac[4] = 0x00;
    mac[5] = 0x00;
}

MBED_WEAK void mbed_die(void)
{
    // Leonardo et al. need interrupts enabled
#ifdef USBCON
    interrupts();
#else
    noInterrupts();
#endif
#ifdef LED_BUILTIN
    gpio_t led;
    gpio_init_out(&led, LED_BUILTIN);
    for (;;) {
        for (int i = 0; i != 8; ++i) {
            int value = gpio_read(&led);
            gpio_write(&led, !value);
            delay_ms(150);
        }
        for (int i = 0; i != 8; ++i) {
            int value = gpio_read(&led);
            gpio_write(&led, !value);
            delay_ms(400);
        }
    }
#endif
}

void mbed_error_puts(const char* message)
{
#ifdef SERIAL_PORT_MONITOR
    core_util_critical_section_enter();
    if (!serial_port_monitor_initialized) {
#ifdef USBCON
        // called from global constructor before main()?
        if (!USBDevice.configured()) {
            init();
            initVariant();
            USBDevice.attach();
        }
        interrupts();
#endif
        SERIAL_PORT_MONITOR.begin(9600);
#ifdef USBCON
        // poll stream every 10ms for max. 3 seconds
        for (int n = 300; !SERIAL_PORT_MONITOR && n != 0; --n) {
            delay_ms(10);
        }
        noInterrupts();
#endif
        serial_port_monitor_initialized = true;
    }
    SERIAL_PORT_MONITOR.write(message);
    SERIAL_PORT_MONITOR.flush();
    core_util_critical_section_exit();
#endif
}

void mbed_error_printf(const char* format, ...)
{
#ifdef SERIAL_PORT_MONITOR
    va_list arg;
    va_start(arg, format);
    mbed_error_vfprintf(format, arg);
    va_end(arg);
#endif
}

void mbed_error_vfprintf(const char* format, va_list arg)
{
#ifdef SERIAL_PORT_MONITOR
    char buffer[ERROR_BUF_SIZE];
    if (vsnprintf(buffer, ERROR_BUF_SIZE, format, arg) >= 0) {
        mbed_error_puts(buffer);
    }
#endif
}

#endif
