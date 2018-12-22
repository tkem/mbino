/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
#ifndef MBINO_AVR_OBJECTS_H
#define MBINO_AVR_OBJECTS_H

#include <stdbool.h>
#include <stdint.h>

#include "PinNames.h"
#include "PortNames.h"
#include "gpio_object.h"

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
    volatile uint8_t *reg;
    PinDirection dir;
    PinMode mode;
    uint8_t value;
    uint8_t port;
    uint8_t mask;
};

typedef struct {
    volatile uint16_t *ocr;
    volatile uint16_t *icr;
    volatile uint8_t *tccra;
    volatile uint8_t *tccrb;
    uint8_t com;
} pwmout16_t;

typedef struct {
    volatile uint8_t *ocr;
    volatile uint8_t *tccr;
    volatile uint8_t *output;
    uint8_t com;
    uint8_t mask;
} pwmout8_t;

typedef union {
    pwmout16_t pwm16;
    pwmout8_t pwm8;
} pwmout_object_t;

typedef struct {
    uint16_t (*read)(pwmout_object_t *obj);
    void (*write)(pwmout_object_t *obj, uint16_t value);
    void (*period)(pwmout_object_t *obj, uint32_t value);
    void (*pulsewidth)(pwmout_object_t *obj, uint32_t value);
    void (*free)(pwmout_object_t *obj);
} pwmout_interface_t;

struct pwmout_s {
    const pwmout_interface_t *interface;
    pwmout_object_t object;
};

struct serial_s {
    uint8_t usart;
    uint8_t txpin;
    volatile uint8_t *udr;
    volatile uint8_t *ucsra;
    volatile uint8_t *ucsrb;
};

struct spi_s {
    uint8_t spr;
};

struct i2c_s {
    bool slave;
};

#endif
