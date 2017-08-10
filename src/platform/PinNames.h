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

#include <pins_arduino.h>

namespace mbino {

    enum PinDirection {
        PIN_INPUT,
        PIN_OUTPUT
    };

    enum PinMode {
        PullNone,
        PullUp,
        PullDefault = PullNone
    };

    // TODO: enum type?
    typedef uint8_t PinName;

    static const PinName NC = -1;

#ifdef NUM_DIGITAL_PINS
    static const PinName p0 = 0;
    static const PinName p1 = 1;
    static const PinName p2 = 2;
#if NUM_DIGITAL_PINS > 3  /* !gemma */
    static const PinName p3 = 3;
    static const PinName p4 = 4;
    static const PinName p5 = 5;
    static const PinName p6 = 6;
    static const PinName p7 = 7;
    static const PinName p8 = 8;
    static const PinName p9 = 9;
    static const PinName p10 = 10;
    static const PinName p11 = 11;
    static const PinName p12 = 12;
    static const PinName p13 = 13;
    static const PinName p14 = 14;
    static const PinName p15 = 15;
    static const PinName p16 = 16;
    static const PinName p17 = 17;
    static const PinName p18 = 18;
    static const PinName p19 = 19;
#endif
#if NUM_DIGITAL_PINS > 20  /* !standard */
    static const PinName p20 = 20;
    static const PinName p21 = 21;
    static const PinName p22 = 22;
    static const PinName p23 = 23;
    static const PinName p24 = 24;
    static const PinName p25 = 25;
    static const PinName p26 = 26;
    static const PinName p27 = 27;
    static const PinName p28 = 28;
    static const PinName p29 = 29;
    static const PinName p30 = 30;
#endif
#if NUM_DIGITAL_PINS > 31  /* !leonardo */
    static const PinName p31 = 31;
    static const PinName p32 = 32;
    static const PinName p33 = 33;
    static const PinName p34 = 34;
    static const PinName p35 = 35;
    static const PinName p36 = 36;
    static const PinName p37 = 37;
    static const PinName p38 = 38;
    static const PinName p39 = 39;
    static const PinName p40 = 40;
    static const PinName p41 = 41;
    static const PinName p42 = 42;
    static const PinName p43 = 43;
    static const PinName p44 = 44;
    static const PinName p45 = 45;
    static const PinName p46 = 46;
    static const PinName p47 = 47;
    static const PinName p48 = 48;
    static const PinName p49 = 49;
    static const PinName p50 = 50;
    static const PinName p51 = 51;
    static const PinName p52 = 52;
    static const PinName p53 = 53;
    static const PinName p54 = 54;
    static const PinName p55 = 55;
    static const PinName p56 = 56;
    static const PinName p57 = 57;
    static const PinName p58 = 58;
    static const PinName p59 = 59;
    static const PinName p60 = 60;
    static const PinName p61 = 61;
    static const PinName p62 = 62;
    static const PinName p63 = 63;
    static const PinName p64 = 64;
    static const PinName p65 = 65;
    static const PinName p66 = 66;
    static const PinName p67 = 67;
    static const PinName p68 = 68;
    static const PinName p69 = 69;
#endif
#if NUM_DIGITAL_PINS > 70  /* !mega */
#warning There are more than 70 digital pins. Some pin names may not be defined.
#endif
#else
#warning The number of digital pins is not defined. No pin names will be available.
#endif

#ifdef LED_BUILTIN
    static const PinName LED1 = LED_BUILTIN;
#else
    static const PinName LED1 = 0;  // FIXME: this is pretty arbitraty
#endif
    // Arduino boards typically have only a single LED
    static const PinName LED2 = LED1;
    static const PinName LED3 = LED2;
    static const PinName LED4 = LED3;

    // TODO: enum type?
    typedef uint8_t PortName;

    static const PortName PortA = 1;
    static const PortName PortB = 2;
    static const PortName PortC = 3;
    static const PortName PortD = 4;
    static const PortName PortE = 5;
    static const PortName PortF = 6;
    static const PortName PortG = 7;
    static const PortName PortH = 8;
    static const PortName PortJ = 10;
    static const PortName PortK = 11;
    static const PortName PortL = 12;

    // TODO: explicit conversion to PinName, not defined on gemma/ATTiny?
    struct USBRX_type {};
    struct USBTX_type {};

    static const USBRX_type USBRX;
    static const USBTX_type USBTX;
}

#endif
