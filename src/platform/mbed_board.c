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
#include "hal/gpio_api.h"
#include "hal/serial_api.h"
#include "platform/mbed_critical.h"
#include "platform/mbed_interface.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_wait_api.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#if DEVICE_SERIAL
serial_t *mbed_stdio_uart = NULL;  // set if already initialized
#endif

#ifndef ERROR_BUF_SIZE
#define ERROR_BUF_SIZE 128
#endif

MBED_WEAK void mbed_mac_address(char *mac)
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
    // FIXME: disable interrupts if possible
    //core_util_critical_section_enter();

    gpio_t led;
    gpio_init_out(&led, LED1);

    for (;;) {
        for (int i = 0; i != 4; ++i) {
            gpio_write(&led, 1);
            wait_ms(150);
            gpio_write(&led, 0);
            wait_ms(150);
        }
        for (int i = 0; i != 4; ++i) {
            gpio_write(&led, 1);
            wait_ms(400);
            gpio_write(&led, 0);
            wait_ms(400);
        }
    }
}

MBED_WEAK void mbed_error_puts(const char* message)
{
#if DEVICE_SERIAL
    static serial_t stdio_uart;
    core_util_critical_section_enter();
    if (!mbed_stdio_uart) {
        serial_init(&stdio_uart, STDIO_UART_TX, STDIO_UART_RX);
        mbed_stdio_uart = &stdio_uart;
    }
    while (*message) {
        serial_putc(mbed_stdio_uart, *message++);
    }
    core_util_critical_section_exit();
#endif
}

void mbed_error_printf(const char *format, ...)
{
#if DEVICE_SERIAL
    va_list arg;
    va_start(arg, format);
    mbed_error_vfprintf(format, arg);
    va_end(arg);
#endif
}

void mbed_error_vfprintf(const char *format, va_list arg)
{
#if DEVICE_SERIAL
    char buffer[ERROR_BUF_SIZE];
    if (vsnprintf(buffer, ERROR_BUF_SIZE, format, arg) >= 0) {
        mbed_error_puts(buffer);
    }
#endif
}
