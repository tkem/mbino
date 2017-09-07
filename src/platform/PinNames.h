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
#ifndef MBINO_PIN_NAMES_H
#define MBINO_PIN_NAMES_H

#include "device.h"

namespace mbino {

    // serial port helpers
    // TODO: move to "impl" namespace?
    struct usb_port {
        struct rx_type {};
        struct tx_type {};
    };

    template<int N>
    struct uart_port {
        // TODO: explicit conversion to PinName?
        struct rx_type {};
        struct tx_type {};
    };

    // TODO: enum type?  how to use A0..An then?
    typedef uint8_t PinName;

    static const PinName NC = -1;

    // assume min. 6 digital pins
    static const PinName D0 = 0;
    static const PinName D1 = 1;
    static const PinName D2 = 2;
    static const PinName D3 = 3;
    static const PinName D4 = 4;
    static const PinName D5 = 5;
#if NUM_DIGITAL_PINS > 6  /* standard */
    static const PinName D6 = 6;
    static const PinName D7 = 7;
    static const PinName D8 = 8;
    static const PinName D9 = 9;
    static const PinName D10 = 10;
    static const PinName D11 = 11;
    static const PinName D12 = 12;
    static const PinName D13 = 13;
    static const PinName D14 = 14;
    static const PinName D15 = 15;
    static const PinName D16 = 16;
    static const PinName D17 = 17;
    static const PinName D18 = 18;
    static const PinName D19 = 19;
#endif
#if NUM_DIGITAL_PINS > 20  /* leonardo */
    static const PinName D20 = 20;
    static const PinName D21 = 21;
    static const PinName D22 = 22;
    static const PinName D23 = 23;
    static const PinName D24 = 24;
    static const PinName D25 = 25;
    static const PinName D26 = 26;
    static const PinName D27 = 27;
    static const PinName D28 = 28;
    static const PinName D29 = 29;
    static const PinName D30 = 30;
#endif
#if NUM_DIGITAL_PINS > 31  /* mega */
    static const PinName D31 = 31;
    static const PinName D32 = 32;
    static const PinName D33 = 33;
    static const PinName D34 = 34;
    static const PinName D35 = 35;
    static const PinName D36 = 36;
    static const PinName D37 = 37;
    static const PinName D38 = 38;
    static const PinName D39 = 39;
    static const PinName D40 = 40;
    static const PinName D41 = 41;
    static const PinName D42 = 42;
    static const PinName D43 = 43;
    static const PinName D44 = 44;
    static const PinName D45 = 45;
    static const PinName D46 = 46;
    static const PinName D47 = 47;
    static const PinName D48 = 48;
    static const PinName D49 = 49;
    static const PinName D50 = 50;
    static const PinName D51 = 51;
    static const PinName D52 = 52;
    static const PinName D53 = 53;
    static const PinName D54 = 54;
    static const PinName D55 = 55;
    static const PinName D56 = 56;
    static const PinName D57 = 57;
    static const PinName D58 = 58;
    static const PinName D59 = 59;
    static const PinName D60 = 60;
    static const PinName D61 = 61;
    static const PinName D62 = 62;
    static const PinName D63 = 63;
    static const PinName D64 = 64;
    static const PinName D65 = 65;
    static const PinName D66 = 66;
    static const PinName D67 = 67;
    static const PinName D68 = 68;
    static const PinName D69 = 69;
#endif
#if NUM_DIGITAL_PINS > 70  /* !mega */
#warning There are more than 70 digital pins. Some pin names may not be defined.
#endif

#ifdef LED_BUILTIN
    static const PinName LED1 = LED_BUILTIN;
#endif

#ifdef DEVICE_I2C
    static const PinName I2C_SCL = PIN_I2C_SCL;
    static const PinName I2C_SDA = PIN_I2C_SDA;
#endif

#ifdef DEVICE_SPI
    static const PinName SPI_MOSI = PIN_SPI_MOSI;
    static const PinName SPI_MISO = PIN_SPI_MISO;
    static const PinName SPI_SCK = PIN_SPI_SCK;
#endif

#ifdef SERIAL_PORT_MONITOR
    static const usb_port::rx_type USBRX;
    static const usb_port::tx_type USBTX;
#endif

#ifdef SERIAL_PORT_HARDWARE
    static const uart_port<0>::rx_type UART_RX;
    static const uart_port<0>::tx_type UART_TX;
    static const uart_port<0>::rx_type UART0_RX;
    static const uart_port<0>::tx_type UART0_TX;
    static const uart_port<0>::rx_type SERIAL_RX;
    static const uart_port<0>::tx_type SERIAL_TX;
#endif

#ifdef SERIAL_PORT_HARDWARE1
    static const uart_port<1>::rx_type UART1_RX;
    static const uart_port<1>::tx_type UART1_TX;
#endif

#ifdef SERIAL_PORT_HARDWARE2
    static const uart_port<2>::rx_type UART2_RX;
    static const uart_port<2>::tx_type UART2_TX;
#endif

#ifdef SERIAL_PORT_HARDWARE3
    static const uart_port<3>::rx_type UART3_RX;
    static const uart_port<3>::tx_type UART3_TX;
#endif
}

#endif
