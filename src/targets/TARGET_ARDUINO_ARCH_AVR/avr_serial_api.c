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

#include "hal/serial_api.h"

#if DEVICE_SERIAL

#include <Arduino.h>

#include "platform/mbino_error.h"

#include "avr_usarts.h"

#define SERIAL_INVARG MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SERIAL, MBED_ERROR_CODE_INVALID_ARGUMENT)

#ifndef MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE 9600
#endif

#ifndef MBED_CONF_PLATFORM_STDIO_BAUD_RATE
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
#endif

#define SERIAL_UBRR(BAUD) (((F_CPU) + 8UL * (BAUD)) / (16UL * (BAUD)) - 1UL)
#define SERIAL_UBRR_U2X(BAUD) (((F_CPU) + 4UL * (BAUD)) / (8UL * (BAUD)) - 1UL)

#if defined(__AVR_ATmega8__)
#define SERIAL_UCSRC(CS, PM, SB) (0x80 | ((CS) << (UCSZ00)) | ((PM) << (UPM00)) | ((SB) << (USBS0)))
#elif defined(UCSZ00) && defined(UPM00) && defined(USBS0)
#define SERIAL_UCSRC(CS, PM, SB) (((CS) << (UCSZ00)) | ((PM) << (UPM00)) | ((SB) << (USBS0)))
#elif defined(UCSZ10) && defined(UPM10) && defined(USBS1)
#define SERIAL_UCSRC(CS, PM, SB) (((CS) << (UCSZ10)) | ((PM) << (UPM10)) | ((SB) << (USBS1)))
#else
#error "Don't know how to set UCSRC"
#endif

/*
  See arduino-1.8.5/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp
*/
#if (F_CPU) == 16000000UL && (MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) == 57600
#define SERIAL_INIT_UBRR SERIAL_UBRR_U2X(MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
#define SERIAL_INIT_UCSRA (1 << U2X)
#elif SERIAL_UBRR(MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE) > 4095
#define SERIAL_INIT_UBRR SERIAL_UBRR_U2X(MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
#define SERIAL_INIT_UCSRA (1 << U2X)
#else
#define SERIAL_INIT_UBRR SERIAL_UBRR(MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE)
#define SERIAL_INIT_UCSRA 0
#endif
#define SERIAL_INIT_UCSRC SERIAL_UCSRC(8 - 5, 0, 1 - 1) /* 8N1 */

int stdio_uart_inited = 0;
serial_t stdio_uart;

static void serial_init_uart(serial_t* obj, uint8_t usart, PinName tx, PinName rx)
{
    obj->usart = usart;
    obj->txpin = tx;
    obj->udr = usartToDataRegister(usart);
    obj->ucsra = usartToControlStatusRegisterA(usart);
    obj->ucsrb = usartToControlStatusRegisterB(usart);

    *obj->ucsra = SERIAL_INIT_UCSRA;
    *usartToBaudRateRegisterH(usart) = SERIAL_INIT_UBRR >> 8;
    *usartToBaudRateRegisterL(usart) = SERIAL_INIT_UBRR;
    *usartToControlStatusRegisterC(usart) = SERIAL_INIT_UCSRC;
    *obj->ucsrb = (tx != NC ? (1 << TXEN) : 0) | (rx != NC ? (1 << RXEN) : 0);

}

void serial_init(serial_t* obj, PinName tx, PinName rx)
{
    if (tx == NC && rx == NC) {
        MBINO_ERROR(SERIAL_INVARG, "Serial pin mapping failed");
#if defined(UDR) || defined(UDR0)
    } else if ((tx == UART0_TX || tx == NC) && (rx == UART0_RX || rx == NC)) {
        serial_init_uart(obj, 0, tx, rx);
#endif
#if defined(UDR1)
    } else if ((tx == UART1_TX || tx == NC) && (rx == UART1_RX || rx == NC)) {
        serial_init_uart(obj, 1, tx, rx);
#endif
#if defined(UDR2)
    } else if ((tx == UART2_TX || tx == NC) && (rx == UART2_RX || rx == NC)) {
        serial_init_uart(obj, 2, tx, rx);
#endif
#if defined(UDR3)
    } else if ((tx == UART3_TX || tx == NC) && (rx == UART3_RX || rx == NC)) {
        serial_init_uart(obj, 3, tx, rx);
#endif
    } else {
        MBINO_ERROR(SERIAL_INVARG, "Serial pin mapping failed");
    }
    if (tx == STDIO_UART_TX && rx == STDIO_UART_RX) {
#if MBED_CONF_PLATFORM_STDIO_BAUD_RATE != MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
        serial_baud(obj, MBED_CONF_PLATFORM_STDIO_BAUD_RATE);
#endif
        stdio_uart = *obj;
        stdio_uart_inited = 1;
    }
}

void serial_free(serial_t* obj)
{
    *obj->ucsrb = 0;
}

void serial_baud(serial_t* obj, long baudrate)
{
    uint16_t ubrr;
    bool u2x;
    if ((F_CPU == 16000000UL && baudrate == 57600) || SERIAL_UBRR(baudrate) > 4095) {
        ubrr = SERIAL_UBRR_U2X(baudrate);
        u2x = true;
    } else {
        ubrr = SERIAL_UBRR(baudrate);
        u2x = false;
    }

    uint8_t sreg = SREG;
    cli();
    if (u2x) {
        *obj->ucsra |= (1 << U2X);
    } else {
        *obj->ucsra &= ~(1 << U2X);
    }
    *usartToBaudRateRegisterH(obj->usart) = ubrr >> 8;
    *usartToBaudRateRegisterL(obj->usart) = ubrr;
    SREG = sreg;
}

void serial_format(serial_t* obj, int data_bits, SerialParity parity, int stop_bits)
{
    // TODO: currently only up to 8 data bits are supported
    uint8_t ucsrc = SERIAL_UCSRC(data_bits - 5, (0x4 - parity) & 0x3, stop_bits - 1);
    *usartToControlStatusRegisterC(obj->usart) = ucsrc;
}

int serial_getc(serial_t* obj)
{
    while (!(*obj->ucsra & (1 << RXC)))
           ;
    uint8_t s = *obj->ucsra;
    uint8_t c = *obj->udr;
    return (s & ((1 << UPE) | (1 << FE))) ? -1 : c;
}

void serial_putc(serial_t* obj, int c)
{
    while (!(*obj->ucsra & (1 << UDRE)))
        ;
    *obj->udr = c;
}

int serial_readable(serial_t* obj)
{
    return (*obj->ucsra & (1 << RXC)) != 0;
}

int serial_writable(serial_t* obj)
{
    return (*obj->ucsra & (1 << UDRE)) != 0;
}

void serial_clear(serial_t *obj)
{
    *obj->udr;
}

void serial_break_set(serial_t *obj)
{
    if (obj->txpin != NC) {
        uint8_t port = digitalPinToPort(obj->txpin);
        uint8_t mask = digitalPinToBitMask(obj->txpin);
        volatile uint8_t *out = portOutputRegister(port);
        volatile uint8_t *mode = portModeRegister(port);
        uint8_t sreg = SREG;
        cli();
        *out &= ~mask;
        *mode |= mask;
        *obj->ucsrb &= ~(1 << TXEN);
        SREG = sreg;
    }
}

void serial_break_clear(serial_t *obj)
{
    if (obj->txpin != NC) {
        uint8_t sreg = SREG;
        cli();
        *obj->ucsrb |= (1 << TXEN);
        SREG = sreg;
    }
}

#endif

#endif
