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
#ifdef ARDUINO_ARCH_SAMD

#include "hal/serial_api.h"

#ifdef DEVICE_SERIAL

#include <Arduino.h>

#include "platform/mbed_error.h"

#if defined(SERIAL_PORT_HARDWARE3)
#define NUM_SERIAL_PORT_HARDWARE 4
#elif defined(SERIAL_PORT_HARDWARE2)
#define NUM_SERIAL_PORT_HARDWARE 3
#elif defined(SERIAL_PORT_HARDWARE1)
#define NUM_SERIAL_PORT_HARDWARE 2
#elif defined(SERIAL_PORT_HARDWARE)
#define NUM_SERIAL_PORT_HARDWARE 1
#else
#define NUM_SERIAL_PORT_HARDWARE 0
#endif

template<class T1, class T2>
bool serial_ptr_eq(const T1* lhs, const T2* rhs)
{
    return false;
}

bool serial_ptr_eq(const HardwareSerial* lhs, const HardwareSerial* rhs)
{
    return lhs == rhs;
}

int stdio_uart_inited = 0;
serial_t stdio_uart;

static uart_irq_handler serial_event_handler = 0;

static serial_t* serial_event_objects[NUM_SERIAL_PORT_HARDWARE];

static const uint16_t serial_config_data[] = {
    HARDSER_DATA_5,
    HARDSER_DATA_6,
    HARDSER_DATA_7,
    HARDSER_DATA_8
};

static const uint16_t serial_config_parity[] = {
    HARDSER_PARITY_NONE,
    HARDSER_PARITY_ODD,
    HARDSER_PARITY_EVEN,
    HARDSER_PARITY_NONE,  // FIXME
    HARDSER_PARITY_NONE   // FIXME
};

static const uint16_t serial_config_stop[] = {
    HARDSER_STOP_BIT_1,
    HARDSER_STOP_BIT_2
};

static void serial_begin(serial_t* obj)
{
    // serial API is not synchronized
    if (!obj->initialized) {
        obj->uart->begin(obj->baudrate, obj->config);
        if (serial_ptr_eq(obj->uart, &SERIAL_PORT_MONITOR)) {
            stdio_uart = *obj;
            stdio_uart_inited = true;
        }
        obj->initialized = true;
    }
}

static void serial_end(serial_t* obj)
{
    // serial API is not synchronized
    if (obj->initialized) {
        obj->uart->end();
        if (serial_ptr_eq(obj->uart, &SERIAL_PORT_MONITOR)) {
            stdio_uart_inited = false;
        }
        obj->initialized = false;
    }
}

static void serial_rx_event(uint8_t n)
{
    if (serial_t* obj = serial_event_objects[n]) {
        if (uart_irq_handler handler = serial_event_handler) {
            handler(obj->irq_id, RxIrq);
        }
    }
}

void serial_init(serial_t* obj, PinName tx, PinName rx)
{
    if (false) {
        // keep the compiler happy
#if (defined PIN_SERIAL_RX) && (defined PIN_SERIAL_TX)
    } else if (tx == UART0_TX && rx == UART0_RX) {
        obj->uart = &Serial;
#endif
#if (defined PIN_SERIAL1_RX) && (defined PIN_SERIAL1_TX)
    } else if (tx == UART1_TX && rx == UART1_RX) {
        obj->uart = &Serial1;
#endif
#if (defined PIN_SERIAL2_TX) && (defined PIN_SERIAL2_RX)
    } else if (tx == UART2_TX && rx == UART2_RX) {
        obj->uart = &Serial2;
#endif
    } else {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_INVALID_ARGUMENT),
                   "Serial pin mapping failed");
    }
    obj->baudrate = 9600;
    obj->config = SERIAL_8N1;
    obj->initialized = false;
}

void serial_free(serial_t* obj)
{
    serial_irq_set(obj, RxIrq, 0);
    serial_irq_set(obj, TxIrq, 0);
    serial_end(obj);
}

void serial_baud(serial_t* obj, int baudrate)
{
    serial_end(obj);
    obj->baudrate = baudrate;
}

void serial_format(serial_t* obj, int data_bits, SerialParity parity, int stop_bits)
{
    serial_end(obj);
    obj->config = serial_config_data[data_bits - 5] |
        serial_config_parity[parity] |
        serial_config_stop[stop_bits - 1];
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, uint32_t id)
{
    // this *really* sets the global handler...
    serial_event_handler = handler;
    obj->irq_id = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, uint32_t enable)
{
    // serial events are synchronous, so no need for locking
    if (irq == RxIrq) {
        if (false) {
            // keep the compiler happy
#ifdef SERIAL_PORT_HARDWARE
        } else if (obj->uart == &SERIAL_PORT_HARDWARE) {
            serial_event_objects[0] = enable ? obj : 0;
#endif
#ifdef SERIAL_PORT_HARDWARE1
        } else if (obj->uart == &SERIAL_PORT_HARDWARE1) {
            serial_event_objects[1] = enable ? obj : 0;
#endif
#ifdef SERIAL_PORT_HARDWARE2
        } else if (obj->uart == &SERIAL_PORT_HARDWARE2) {
            serial_event_objects[2] = enable ? obj : 0;
#endif
#ifdef SERIAL_PORT_HARDWARE3
        } else if (obj->uart == &SERIAL_PORT_HARDWARE3) {
            serial_event_objects[3] = enable ? obj : 0;
#endif
        } else {
            MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_INVALID_ARGUMENT),
                       "Serial IRQ mapping failed");
        }
    } else if (enable) {
        MBED_ERROR(MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_UNSUPPORTED),
                   "Serial TxIrq not supported");
    }
    serial_begin(obj);
}

int serial_getc(serial_t* obj)
{
    // mbed getc() is blocking, while Arduino Stream::read() is not
    serial_begin(obj);
    int c;
    do {
        c = obj->uart->read();
    } while (c < 0);
    return c;
}

void serial_putc(serial_t* obj, int c)
{
    // mbed putc() is unbuffered (needs to work w/o interrupts)
    serial_begin(obj);
    obj->uart->write(c);
    obj->uart->flush();
}

int serial_readable(serial_t* obj)
{
    return obj->uart->available() != 0;
}

int serial_writable(serial_t* obj)
{
    return obj->uart->availableForWrite() != 0;
}

void serial_clear(serial_t *obj)
{
    // FIXME: What does "clear" mean?
}

void serial_break_set(serial_t *obj)
{
    // FIXME: pin handling
    serial_end(obj);
}

void serial_break_clear(serial_t *obj)
{
    // FIXME: pin handling
    serial_begin(obj);
}

#ifdef SERIAL_PORT_HARDWARE
void serialEvent()
{
    serial_rx_event(0);
}
#endif

#ifdef SERIAL_PORT_HARDWARE1
void serialEvent1()
{
    serial_rx_event(1);
}
#endif

#ifdef SERIAL_PORT_HARDWARE2
void serialEvent2()
{
    serial_rx_event(2);
}
#endif

#ifdef SERIAL_PORT_HARDWARE3
void serialEvent3()
{
    serial_rx_event(3);
}
#endif

#endif

#endif
