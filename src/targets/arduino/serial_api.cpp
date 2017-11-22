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
#include "hal/serial_api.h"

#ifdef DEVICE_SERIAL

#include "platform/mbed_error.h"

#include <Arduino.h>

#if defined(SERIAL_PORT_HARDWARE3)
#define NUM_HARDWARE_SERIAL_PORTS 4
#elif defined(SERIAL_PORT_HARDWARE2)
#define NUM_HARDWARE_SERIAL_PORTS 3
#elif defined(SERIAL_PORT_HARDWARE1)
#define NUM_HARDWARE_SERIAL_PORTS 2
#elif defined(SERIAL_PORT_HARDWARE)
#define NUM_HARDWARE_SERIAL_PORTS 1
#else
#define NUM_HARDWARE_SERIAL_PORTS 0
#endif

extern "C" {
    extern serial_t *mbed_stdio_uart;
}

static uart_irq_handler event_handler = 0;

static serial_t* event_objects[NUM_HARDWARE_SERIAL_PORTS];

template<class T>
static void serial_init(serial_t* obj, T* stream)
{
    obj->interface = &serial_stream<T>::interface;
    obj->stream = stream;
    obj->baudrate = 9600;
    obj->cs = 8 - 5;
    obj->pm = ParityNone;
    obj->sb = 1 - 1;
    obj->initialized = false;
}

static void serial_begin(serial_t* obj)
{
    // serial API is not synchronized
    if (!obj->initialized) {
        obj->interface->begin(obj->stream, obj->baudrate, obj->cs, obj->pm, obj->sb);
        obj->initialized = true;
    }
}

static void serial_reset(serial_t* obj)
{
    // serial API is not synchronized
    if (obj->initialized) {
        obj->stream->flush();
        obj->interface->begin(obj->stream, obj->baudrate, obj->cs, obj->pm, obj->sb);
    }
}

static void serial_rx_event(uint8_t n)
{
    if (serial_t* obj = event_objects[n]) {
        event_handler(obj->irq_id, RxIrq);
    }
}

void serial_init(serial_t* obj, PinName tx, PinName rx)
{
    if (false) {
        // keep the compiler happy
#ifdef SERIAL_PORT_USBVIRTUAL
    } else if (tx == USBTX && rx == USBRX) {
        serial_init(obj, &SERIAL_PORT_USBVIRTUAL);
#endif
#ifdef SERIAL_PORT_HARDWARE
    } else if (tx == UART0_TX && rx == UART0_RX) {
        serial_init(obj, &SERIAL_PORT_HARDWARE);
#endif
#ifdef SERIAL_PORT_HARDWARE1
    } else if (tx == UART1_TX && rx == UART1_RX) {
        serial_init(obj, &SERIAL_PORT_HARDWARE1);
#endif
#ifdef SERIAL_PORT_HARDWARE2
    } else if (tx == UART2_TX && rx == UART2_RX) {
        serial_init(obj, &SERIAL_PORT_HARDWARE2);
#endif
#ifdef SERIAL_PORT_HARDWARE3
    } else if (tx == UART3_TX && rx == UART3_RX) {
        serial_init(obj, &SERIAL_PORT_HARDWARE3);
#endif
    } else {
        error("Serial pin mapping failed");
    }
    if (obj->stream == &SERIAL_PORT_MONITOR) {
        mbed_stdio_uart = obj;
    }
}

void serial_free(serial_t* obj)
{
    if (obj == mbed_stdio_uart) {
        mbed_stdio_uart = NULL;
    }
    serial_irq_set(obj, RxIrq, 0);
    serial_irq_set(obj, TxIrq, 0);
    obj->interface->end(obj->stream);
}

void serial_baud(serial_t* obj, long baudrate)
{
    obj->baudrate = baudrate;
    serial_reset(obj);
}

void serial_format(serial_t* obj, int data_bits, SerialParity parity, int stop_bits)
{
    obj->cs = data_bits - 5;
    obj->pm = parity;
    obj->sb = stop_bits - 1;
    serial_reset(obj);
}

void serial_irq_handler(serial_t *obj, uart_irq_handler handler, intptr_t id)
{
    // this *really* sets the global handler...
    event_handler = handler;
    obj->irq_id = id;
}

void serial_irq_set(serial_t *obj, SerialIrq irq, int enable)
{
    serial_begin(obj);
    // serial events are synchronous, so no need for locking
    if (irq == RxIrq) {
        if (false) {
            // keep the compiler happy
#ifdef SERIAL_PORT_HARDWARE
        } else if (obj->stream == &SERIAL_PORT_HARDWARE) {
            event_objects[0] = enable ? obj : 0;
#endif
#ifdef SERIAL_PORT_HARDWARE1
        } else if (obj->stream == &SERIAL_PORT_HARDWARE1) {
            event_objects[1] = enable ? obj : 0;
#endif
#ifdef SERIAL_PORT_HARDWARE2
        } else if (obj->stream == &SERIAL_PORT_HARDWARE2) {
            event_objects[2] = enable ? obj : 0;
#endif
#ifdef SERIAL_PORT_HARDWARE3
        } else if (obj->stream == &SERIAL_PORT_HARDWARE3) {
            event_objects[3] = enable ? obj : 0;
#endif
        } else {
            error("Serial IRQ mapping failed");
        }
    } else {
        error("Serial TxIrq not supported");
    }
}

int serial_getc(serial_t* obj)
{
    // mbed getc() is blocking, while Arduino Stream::read() is not
    serial_begin(obj);
    int c;
    do {
        c = obj->stream->read();
    } while (c < 0);
    return c;
}

void serial_putc(serial_t* obj, int c)
{
    serial_begin(obj);
    obj->stream->write(c);
}

int serial_readable(serial_t* obj)
{
    return obj->stream->available() != 0;
}

// FIXME: Print::availableForWrite() not available on SAM and Trinket M0?
#ifdef ARDUINO_ARCH_AVR
int serial_writable(serial_t* obj)
{
    return obj->stream->availableForWrite() != 0;
}
#endif

void serial_clear(serial_t *obj)
{
    if (obj->initialized) {
        // not really "clear", but close
        obj->stream->flush();
    }
}

void serial_break_set(serial_t *obj)
{
    if (obj->initialized) {
        obj->interface->end(obj->stream);
        // TODO: set TX high?
    }
}

void serial_break_clear(serial_t *obj)
{
    if (obj->initialized) {
        obj->interface->begin(obj->stream, obj->baudrate, obj->cs, obj->pm, obj->sb);
    }
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
