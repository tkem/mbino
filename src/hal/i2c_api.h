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
#ifndef MBINO_I2C_API_H
#define MBINO_I2C_API_H

#include "device.h"

#ifdef DEVICE_I2C

#if DEVICE_I2C_ASYNCH
#error "I2C asynchronous operation not supported."
#else
typedef struct i2c_s i2c_t;
#endif

enum {
    I2C_ERROR_NO_SLAVE = -1,
    I2C_ERROR_BUS_BUSY = -2
};

#ifdef __cplusplus
extern "C" {
#endif

void i2c_init(i2c_t *obj, PinName sda, PinName scl);

// mbino extension: change hz type to long
void i2c_frequency(i2c_t *obj, long hz);

int i2c_start(i2c_t *obj);

int i2c_stop(i2c_t *obj);

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop);

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop);

void i2c_reset(i2c_t *obj);

int i2c_byte_read(i2c_t *obj, int last);

int i2c_byte_write(i2c_t *obj, int data);

#if DEVICE_I2CSLAVE
#error "I2C slave operation not supported."
#endif

#if DEVICE_I2C_ASYNCH
#error "I2C asynchronous operation not supported."
#endif

#ifdef __cplusplus
}
#endif

#endif

#endif
