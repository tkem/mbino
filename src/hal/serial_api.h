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

#include "device.h"

#ifdef DEVICE_SERIAL

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ParityNone = 0,
    ParityOdd = 1,
    ParityEven = 2
} SerialParity;

typedef struct serial_s serial_t;

void serial_init(serial_t *obj, PinName tx, PinName rx);

// mbino extension
void serial_monitor_init(serial_t *obj);

void serial_free(serial_t *obj);

// mbino extension: change baudrate type to long
void serial_baud(serial_t *obj, long baudrate);

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits);

// TODO: serial_irq_handler, serial_irq_set

int serial_getc(serial_t *obj);

void serial_putc(serial_t *obj, int c);

// mbino extension
void serial_puts(serial_t *obj, const char *s);

int serial_readable(serial_t *obj);

int serial_writable(serial_t *obj);

// TODO: serial_clear, serial_break_set, serial_break_clear, serial_set_flow_control, ...

#ifdef __cplusplus
}
#endif

#endif

#endif
