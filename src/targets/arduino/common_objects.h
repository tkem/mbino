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
#ifndef MBINO_COMMON_OBJECTS_H
#define MBINO_COMMON_OBJECTS_H

#if defined(ARDUINO_ARCH_AVR)
#include "avr/gpio_object.h"
#else
#error “This library only supports boards with an AVR processor.”
#endif

#include "i2c_object.h"
#include "tickers.h"

#include <stdbool.h>
#include <stdint.h>

struct Stream; // forward declaration of Arduino Stream class

struct analogin_s {
    uint8_t pin;
};

struct gpio_irq_s {
    gpio_t gpio;
    intptr_t id;
    uint8_t irq;
    uint8_t events;
    bool enabled;
};

typedef struct {
    void (*begin)(struct Stream *obj, long baud, uint8_t format);
    void (*end)(struct Stream *obj);
} serial_stream_interface_t;

struct serial_s {
    const serial_stream_interface_t *interface;
    struct Stream *stream;
    intptr_t irq_id;
    long baudrate;
    uint8_t format;
    bool initialized;
};

struct spi_s {
    uint32_t clock;
    uint8_t bits;
    uint8_t mode;
    // cannot use SPISettings class in C interface
    union {
        int i;
        char c[6];  // needed on SAM processors
    } settings;
};

#endif
