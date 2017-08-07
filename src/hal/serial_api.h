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

class Stream; // FIXME: forward declaration of Arduino Stream type

// TBD: extern "C"?
namespace mbino {

    typedef enum {
        ParityNone = 0,
        ParityEven = 2,
        ParityOdd = 3
    } SerialParity;
/*
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

    typedef void (*uart_irq_handler)(uint32_t id, SerialIrq event);
*/

    struct serial_t {
        Stream* stream;
        PinName tx;
        PinName rx;
        long baudrate;
        uint8_t config;
    };

    void serial_init(serial_t* obj, PinName tx, PinName rx);

    void serial_usb_init(serial_t* obj);

    void serial_free(serial_t* obj);

    void serial_baud(serial_t* obj, long baudrate);

    void serial_format(serial_t* obj, uint8_t data_bits, SerialParity parity, uint8_t stop_bits);
/*
    void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id);

    void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable);
*/
    int serial_getc(serial_t* obj);

    void serial_putc(serial_t* obj, int c);

    void serial_puts(serial_t* obj, const char* s);

    bool serial_readable(serial_t* obj);

    bool serial_writable(serial_t* obj);

/*
    void serial_clear(serial_t* obj);

    void serial_break_set(serial_t *obj);

    void serial_break_clear(serial_t *obj);

    void serial_pinout_tx(PinName tx);

    void serial_set_flow_control(serial_t *obj, FlowControl type, PinName rxflow, PinName txflow);
*/

}

#endif
