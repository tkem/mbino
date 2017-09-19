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
#ifndef MBINO_AVR_OBJECTS_H
#define MBINO_AVR_OBJECTS_H

#include "PortNames.h"
#include "gpio_object.h"

#include <stdint.h>

#define DEVICE_INTERRUPTIN 1
#define DEVICE_PORTIN 1
#define DEVICE_PORTINOUT 1
#define DEVICE_PORTOUT 1
#define DEVICE_PWMOUT 1

struct port_s {
    volatile uint8_t *reg;
    uint8_t port;
    uint8_t mask;
};

typedef enum {
    IRQ_NONE,
    IRQ_RISE,
    IRQ_FALL
} gpio_irq_event;

struct gpio_irq_s {
    void (*handler)(intptr_t id, gpio_irq_event event);
    intptr_t id;
    const volatile uint8_t *reg;
    uint8_t mask;
    uint8_t irq;
    uint8_t events;
    uint8_t enabled;
};

typedef struct {
    volatile uint8_t *tccra;
    volatile uint8_t *tccrb;
    volatile uint16_t *ocr;
    volatile uint16_t *icr;
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
} pwmout_interface_t;

struct pwmout_s {
    const pwmout_interface_t *interface;
    pwmout_object_t object;
};

#endif
