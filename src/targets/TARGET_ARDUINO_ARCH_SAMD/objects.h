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
#ifndef MBINO_SAMD_OBJECTS_H
#define MBINO_SAMD_OBJECTS_H

#include <stdint.h>

#include "PinNames.h"
#include "PortNames.h"
#include "gpio_object.h"

// forward declarations of Arduino C++ classes
struct HardwareSerial;
struct SPIClass;
struct TwoWire;

struct analogin_s {
    uint8_t pin;
};

struct gpio_irq_s {
    gpio_t gpio;
    uint32_t id;
    uint8_t irq;
    uint8_t events;
    bool enabled;
};

struct port_s {
    PortName port;
    uint32_t mask;
};

typedef struct {
    uint16_t (*read)(unsigned channel);
    void (*write)(unsigned channel, uint16_t value);
    void (*period)(unsigned channel, uint32_t value);
    void (*pulsewidth)(unsigned channel, uint32_t value);
} pwmout_interface_t;

struct pwmout_s {
    const pwmout_interface_t *interface;
    unsigned channel;
};

struct serial_s {
    struct HardwareSerial *uart;
    intptr_t irq_id;
    int baudrate;
    uint16_t config;
    bool initialized;
};

#endif
