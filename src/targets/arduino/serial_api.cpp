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

static uart_irq_handler event_handler = 0;

static serial_t* event_objects[NUM_HARDWARE_SERIAL_PORTS];

static void serial_rx_event(uint8_t n)
{
    if (serial_t* obj = event_objects[n]) {
        event_handler(obj->irq_id, RxIrq);
    }
}

template<class T, typename U>
static void serial_stream_begin_config(T* stream, long baud, U config)
{
    stream->begin(baud, config);
}

#ifdef ARDUINO_ARCH_SAM
static void serial_stream_begin_config(UARTClass* stream, long baud, USARTClass::USARTModes)
{
    stream->begin(baud);
}
#endif

template<class T>
static void serial_stream_begin(Stream* obj, long baud, uint8_t format)
{
    T* stream = static_cast<T*>(obj);
    switch (format) {
    case 000:
        serial_stream_begin_config(stream, baud, SERIAL_5N1);
        break;
    case 001:
        serial_stream_begin_config(stream, baud, SERIAL_6N1);
        break;
    case 002:
        serial_stream_begin_config(stream, baud, SERIAL_7N1);
        break;
    case 003:
        serial_stream_begin_config(stream, baud, SERIAL_8N1);
        break;
    case 004:
        serial_stream_begin_config(stream, baud, SERIAL_5N2);
        break;
    case 005:
        serial_stream_begin_config(stream, baud, SERIAL_6N2);
        break;
    case 006:
        serial_stream_begin_config(stream, baud, SERIAL_7N2);
        break;
    case 007:
        serial_stream_begin_config(stream, baud, SERIAL_8N2);
        break;
    case 010:
        serial_stream_begin_config(stream, baud, SERIAL_5O1);
        break;
    case 011:
        serial_stream_begin_config(stream, baud, SERIAL_6O1);
        break;
    case 012:
        serial_stream_begin_config(stream, baud, SERIAL_7O1);
        break;
    case 013:
        serial_stream_begin_config(stream, baud, SERIAL_8O1);
        break;
    case 014:
        serial_stream_begin_config(stream, baud, SERIAL_5O2);
        break;
    case 015:
        serial_stream_begin_config(stream, baud, SERIAL_6O2);
        break;
    case 016:
        serial_stream_begin_config(stream, baud, SERIAL_7O2);
        break;
    case 017:
        serial_stream_begin_config(stream, baud, SERIAL_8O2);
        break;
    case 020:
        serial_stream_begin_config(stream, baud, SERIAL_5E1);
        break;
    case 021:
        serial_stream_begin_config(stream, baud, SERIAL_6E1);
        break;
    case 022:
        serial_stream_begin_config(stream, baud, SERIAL_7E1);
        break;
    case 023:
        serial_stream_begin_config(stream, baud, SERIAL_8E1);
        break;
    case 024:
        serial_stream_begin_config(stream, baud, SERIAL_5E2);
        break;
    case 025:
        serial_stream_begin_config(stream, baud, SERIAL_6E2);
        break;
    case 026:
        serial_stream_begin_config(stream, baud, SERIAL_7E2);
        break;
    case 027:
        serial_stream_begin_config(stream, baud, SERIAL_8E2);
        break;
    default:
        stream->begin(baud);
    }
    // wait for serial port to become ready
    while (!*stream)
        ;
}

template<class T>
static void serial_stream_end(Stream* obj)
{
    static_cast<T*>(obj)->end();
}

template<class T>
static void serial_init(serial_t* obj, T* stream)
{
    static const serial_stream_interface_t interface = {
        &serial_stream_begin<T>,
        &serial_stream_end<T>
    };
    obj->interface = &interface;
    obj->stream = stream;
    obj->baudrate = 9600;
    obj->format = (8 - 5) | ((1 - 1) << 2) | (ParityNone << 3);
    obj->initialized = false;
}

static void serial_begin(serial_t* obj)
{
    // serial API is not synchronized
    if (!obj->initialized) {
        obj->interface->begin(obj->stream, obj->baudrate, obj->format);
        obj->initialized = true;
    }
}

static void serial_reset(serial_t* obj)
{
    // serial API is not synchronized
    if (obj->initialized) {
        obj->stream->flush();
        obj->interface->begin(obj->stream, obj->baudrate, obj->format);
    }
}

#ifdef SERIAL_PORT_USBVIRTUAL
void serial_usb_init(serial_t* obj)
{
    serial_init(obj, &SERIAL_PORT_USBVIRTUAL);
}
#endif

void serial_init(serial_t* obj, PinName tx, PinName rx)
{
    if (false) {
        // keep the compiler happy
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
    }
}

void serial_free(serial_t* obj)
{
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
    obj->format = (data_bits - 5) | ((stop_bits - 1) << 2) | (parity << 3);
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
        }
    } else {
        // TxIrq not supported
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

// FIXME: Print::availableForWrite() not available on SAM?
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
        obj->interface->begin(obj->stream, obj->baudrate, obj->format);
    }
}

// FIXME: what is this?
void serial_pinout_tx(PinName tx);

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
