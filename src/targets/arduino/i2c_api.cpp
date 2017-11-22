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
#ifndef ARDUINO_ARCH_AVR

#include "hal/i2c_api.h"

#ifdef DEVICE_I2C

#include "platform/mbed_error.h"

#include <Wire.h>

void i2c_init(i2c_t* obj, PinName sda, PinName scl)
{
    if (sda == PIN_WIRE_SDA && scl == PIN_WIRE_SCL) {
        obj->wire = &Wire;
#if WIRE_INTERFACES_COUNT > 1
    } else if (sda == PIN_WIRE1_SDA && scl == PIN_WIRE1_SCL) {
        obj->wire = &Wire1;
#endif
#if WIRE_INTERFACES_COUNT > 2
    } else if (sda == PIN_WIRE2_SDA && scl == PIN_WIRE2_SCL) {
        obj->wire = &Wire2;
#endif
#if WIRE_INTERFACES_COUNT > 3
    } else if (sda == PIN_WIRE3_SDA && scl == PIN_WIRE3_SCL) {
        obj->wire = &Wire3;
#endif
#if WIRE_INTERFACES_COUNT > 4
    } else if (sda == PIN_WIRE4_SDA && scl == PIN_WIRE4_SCL) {
        obj->wire = &Wire4;
#endif
#if WIRE_INTERFACES_COUNT > 5
    } else if (sda == PIN_WIRE5_SDA && scl == PIN_WIRE5_SCL) {
        obj->wire = &Wire5;
#endif
    } else {
        error("I2C pin mapping failed");
    }
    obj->wire->begin();
}

void i2c_frequency(i2c_t* obj, long hz)
{
    obj->wire->setClock(hz);
}

int i2c_read(i2c_t* obj, int address, char* data, int length, int stop)
{
    // FIXME: nread > length?
    uint8_t nread = obj->wire->requestFrom(address >> 1, length, stop);
    int n = 0;
    // FIXME: available() < nread?
    while (n != nread && obj->wire->available()) {
        data[n++] = obj->wire->read();
    }
    return n;
}

int i2c_write(i2c_t* obj, int address, const char* data, int length, int stop)
{
    obj->wire->beginTransmission(address >> 1);
    int n = obj->wire->write(data, length);
    // FIXME: error mapping?
    switch (obj->wire->endTransmission(stop)) {
    case 0:
        return n;
    case 1:
        return n;  // ???
    case 2:
        return -1;
    case 3:
        return -1;  // ???
    default:
        return -2;  // ???
    }
}

// TODO: other i2c functions

#endif

#endif
