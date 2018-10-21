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
#ifndef MBINO_SAMD_PIN_NAMES_H
#define MBINO_SAMD_PIN_NAMES_H

#include <Arduino.h>

typedef enum {
    PIN_INPUT,
    PIN_OUTPUT
} PinDirection;

typedef enum {
    PullNone = INPUT,
    PullUp = INPUT_PULLUP,
    PullDown = INPUT_PULLDOWN,
    PullDefault = PullNone
} PinMode;

// *not* an enum type so predefined A0..An can be used as well
typedef uint8_t PinName;

static const PinName NC = -1;

// assume min. 15 digital pins
static const PinName D0 = 0;
static const PinName D1 = 1;
static const PinName D2 = 2;
static const PinName D3 = 3;
static const PinName D4 = 4;
static const PinName D5 = 5;
static const PinName D6 = 6;
static const PinName D7 = 7;
static const PinName D8 = 8;
static const PinName D9 = 9;
static const PinName D10 = 10;
static const PinName D11 = 11;
static const PinName D12 = 12;
static const PinName D13 = 13;
static const PinName D14 = 14;
#if NUM_DIGITAL_PINS > 15  /* mzero, zero, mkr1000 */
static const PinName D15 = 15;
static const PinName D16 = 16;
static const PinName D17 = 17;
static const PinName D18 = 18;
#endif
#if NUM_DIGITAL_PINS > 19  /* zero, mkr1000 */
static const PinName D19 = 19;
#endif
#if NUM_DIGITAL_PINS > 20  /* mkr1000 */
static const PinName D20 = 20;
static const PinName D21 = 21;
#endif
#if NUM_DIGITAL_PINS > 22
#warning There are more than 22 digital pins on your board. Some pin names may not be defined.
#endif

#ifdef PIN_LED
static const PinName LED1 = PIN_LED;
#ifdef PIN_LED2
static const PinName LED2 = PIN_LED2;
#else
static const PinName LED2 = PIN_LED;
#endif
#ifdef PIN_LED3
static const PinName LED3 = PIN_LED3;
#else
static const PinName LED3 = PIN_LED;
#endif
#ifdef PIN_LED4
static const PinName LED4 = PIN_LED4;
#else
static const PinName LED4 = PIN_LED;
#endif
#endif

#ifdef PIN_WIRE_SCL
static const PinName I2C_SCL = PIN_WIRE_SCL;
#endif
#ifdef PIN_WIRE_SDA
static const PinName I2C_SDA = PIN_WIRE_SDA;
#endif

#ifdef PIN_SPI_MISO
static const PinName SPI_MOSI = PIN_SPI_MOSI;
#endif
#ifdef PIN_SPI_MOSI
static const PinName SPI_MISO = PIN_SPI_MISO;
#endif
#ifdef PIN_SPI_SCK
static const PinName SPI_SCK = PIN_SPI_SCK;
#endif

#if (defined PIN_SERIAL_RX) && (defined PIN_SERIAL_TX)
static const PinName SERIAL_RX = PIN_SERIAL_RX;
static const PinName SERIAL_TX = PIN_SERIAL_TX;
static const PinName UART_RX = PIN_SERIAL_RX;
static const PinName UART_TX = PIN_SERIAL_TX;
static const PinName UART0_RX = PIN_SERIAL_RX;
static const PinName UART0_TX = PIN_SERIAL_TX;
#define STDIO_UART_RX SERIAL_RX
#define STDIO_UART_TX SERIAL_TX
#endif
#if (defined PIN_SERIAL1_RX) && (defined PIN_SERIAL1_TX)
static const PinName UART1_RX = PIN_SERIAL1_RX;
static const PinName UART1_TX = PIN_SERIAL1_TX;
#endif
#if (defined PIN_SERIAL2_rX) && (defined PIN_SERIAL2_TX)
static const PinName UART2_RX = PIN_SERIAL2_RX;
static const PinName UART2_TX = PIN_SERIAL2_TX;
#endif

// FIXME: extern?
#if defined (SERIAL_PORT_MONITOR) && !defined(SERIAL_PORT_USBVIRTUAL)
static const PinName USBRX = 0;
static const PinName USBTX = 1;
#endif

#endif
