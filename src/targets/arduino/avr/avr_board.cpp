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

#include "avr_timers.h"

#include <Arduino.h>

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef ERROR_BUF_SIZE
#define ERROR_BUF_SIZE 128
#endif

#if DEVICE_SERIAL
extern bool serial_port_monitor_initialized;
#endif

template<int N>
static void delay_ms()
{
    // FIXME: enable only interrupts necessary for Leonardo USB port?
#ifdef USBCON
    interrupts();
#endif
    // "Currently, the largest value that will produce an accurate delay is 16383."
    for (int i = max((N * 1000L) / 16383L, 1); i != 0; --i) {
        delayMicroseconds(16383);
    }
#ifdef USBCON
    noInterrupts();
#endif
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
    noInterrupts();
#ifdef LED_BUILTIN
    uint8_t port = digitalPinToPort(LED_BUILTIN);
    uint8_t mask = digitalPinToBitMask(LED_BUILTIN);
    volatile uint8_t *output = portOutputRegister(port);
    if (digitalPinHasPWM(LED_BUILTIN)) {
        uint8_t timer = digitalPinToTimer(LED_BUILTIN);
        *timerToControlRegister(timer) &= ~timerToCompareOutputModeMask(timer);
    }
    *portModeRegister(port) |= mask;

    for (;;) {
        for (int i = 0; i != 4; ++i) {
            *output |= mask;
            delay_ms<150>();
            *output &= ~mask;
            delay_ms<150>();
        }
        for (int i = 0; i != 4; ++i) {
            *output |= mask;
            delay_ms<400>();
            *output &= ~mask;
            delay_ms<400>();
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
        // FIXME: initialize Leonardo USBDevice properly...
        if (!USBDevice.configured()) {
            init();
            initVariant();
            USBDevice.attach();
        }
        interrupts();
#endif
        SERIAL_PORT_MONITOR.begin(9600);
#ifdef USBCON
        // FIXME: poll stream every ~100ms for max. 3 seconds
        for (int n = 30; !SERIAL_PORT_MONITOR && n != 0; --n) {
            delay_ms<100>();
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
