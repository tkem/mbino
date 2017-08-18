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
#include "i2c_api.h"

#ifdef DEVICE_I2C

#include <Wire.h>

namespace mbino {

    void i2c_init(i2c_t* obj)
    {
        // FIXME: on first call? Prevent multiple initialization?
        Wire.begin();
    }

    void i2c_frequency(i2c_t* obj, long hz)
    {
        Wire.setClock(hz);
    }

    int i2c_read(i2c_t* obj, uint8_t address, char* data, uint8_t length, bool stop)
    {
        // FIXME: nread > length?
        uint8_t nread = Wire.requestFrom(uint8_t(address >> 1), length, uint8_t(stop));
        int n = 0;
        // FIXME: available() < nread?
        while (n != nread && Wire.available()) {
            data[n++] = Wire.read();
        }
        return n;
    }

    int i2c_write(i2c_t* obj, uint8_t address, const char* data, uint8_t length, bool stop)
    {
        Wire.beginTransmission(address >> 1);
        int n = Wire.write(data, length);
        // FIXME: error mapping?
        switch (Wire.endTransmission(stop)) {
        case 0:
            return n;
        case 1:
            return n;  // ???
        case 2:
            return I2C_ERROR_NO_SLAVE;
        case 3:
            return I2C_ERROR_NO_SLAVE;  // ???
        default:
            return I2C_ERROR_BUS_BUSY;  // ???
        }
    }

}

#endif
