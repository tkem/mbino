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

#include <stdint.h>

#ifdef DEVICE_SERIAL

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ParityNone = 0,
    ParityOdd = 1,
    ParityEven = 2,
    ParityForced1 = 3,
    ParityForced0 = 4
} SerialParity;

typedef enum {
    RxIrq,
    TxIrq
} SerialIrq;

typedef enum {
    FlowControlNone,
    FlowControlRTS,
    FlowControlCTS,
    FlowControlRTSCTS
} FlowControl;

typedef void (*uart_irq_handler)(intptr_t id, SerialIrq event);

#if DEVICE_SERIAL_ASYNCH
#error "Serial asynchronous operation not supported."
#else
typedef struct serial_s serial_t;
#endif

// mbino extension for Arduino "virtual" serial USB port
void serial_usb_init(serial_t *obj);

void serial_init(serial_t *obj, PinName tx, PinName rx);

void serial_free(serial_t *obj);

// mbino extension: change baudrate type to long
void serial_baud(serial_t *obj, long baudrate);

void serial_format(serial_t *obj, int data_bits, SerialParity parity, int stop_bits);

// mbino extension: change id type to intptr_t
void serial_irq_handler(serial_t *obj, uart_irq_handler handler, intptr_t id);

// mbino extension: change enable type to int
void serial_irq_set(serial_t *obj, SerialIrq irq, int enable);

int serial_getc(serial_t *obj);

void serial_putc(serial_t *obj, int c);

int serial_readable(serial_t *obj);

int serial_writable(serial_t *obj);

void serial_clear(serial_t *obj);

void serial_break_set(serial_t *obj);

void serial_break_clear(serial_t *obj);

void serial_pinout_tx(PinName tx);

void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow);

#if DEVICE_SERIAL_ASYNCH
#error "Serial asynchronous operation not supported."
#endif

#if DEVICE_STDIO_MESSAGES
// mbino extension: mbed does not make these public; we also provide
// an explicit init function instead of an "inited" flag
extern serial_t stdio_uart;
void stdio_uart_init(void);
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif
