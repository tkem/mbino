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
#ifndef MBINO_PORT_API_H
#define MBINO_PORT_API_H

#include "device.h"

#if DEVICE_PORTIN || DEVICE_PORTOUT

#ifdef __cplusplus
extern "C" {
#endif

typedef struct port_s port_t;

void port_init(port_t *obj, PortName port, int mask, PinDirection direction);

// mbino extension
void port_init_in(port_t *obj, PortName port, int mask);

// mbino extension
void port_init_in_ex(port_t *obj, PortName port, int mask, PinMode mode);

// mbino extension
void port_init_out(port_t *obj, PortName port, int mask);

// mbino extension
void port_init_out_ex(port_t *obj, PortName port, int mask, int value);

void port_dir(port_t *obj, PinDirection direction);

// mbino extension
void port_dir_in(port_t *obj, PinMode mode);

// mbino extension
void port_dir_out(port_t *obj, int value);

void port_write(port_t *obj, int value);

int port_read(port_t *obj);

void port_mode(port_t *obj, PinMode mode);

#ifdef __cplusplus
}
#endif

#endif

#endif
