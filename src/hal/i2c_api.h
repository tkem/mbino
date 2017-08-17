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

#include "platform/platform.h"

// TBD: extern "C"?
namespace mbino {

    enum {
        I2C_ERROR_NO_SLAVE = -1,
        I2C_ERROR_BUS_BUSY = -2
    };

    struct i2c_t {};

    void i2c_init(i2c_t* obj);

    void i2c_frequency(i2c_t* obj, long hz);

    int i2c_read(i2c_t* obj, uint8_t address, char* data, uint8_t length, bool stop);

    int i2c_write(i2c_t* obj, uint8_t address, const char* data, uint8_t length, bool stop);

}

#endif
