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
#ifndef MBINO_I2C_H
#define MBINO_I2C_H

#include "platform/platform.h"

#if defined(DEVICE_I2C) || defined(DOXYGEN_ONLY)

#include "hal/i2c_api.h"
#include "platform/NonCopyable.h"

namespace mbino {

    class I2C : private NonCopyable<I2C> {
        i2c_t _i2c;

    public:

        /* TODO: what's this for?
        enum RxStatus {
            NoData,
            MasterGeneralCall,
            MasterWrite,
            MasterRead
        };

        enum Acknowledge {
            NoACK = 0,
            ACK = 1
        };
        */

        I2C(PinName sda, PinName scl)  {
            i2c_init(&_i2c, sda, scl);
        }

        // mbino extension: change hz type to long
        void frequency(long hz) {
            i2c_frequency(&_i2c, hz);
        }

        int read(int address, char* data, int length, bool repeated = false) {
            return i2c_read(&_i2c, address, data, length, !repeated);
        }

        int read(int ack) {
            return i2c_byte_read(&_i2c, !ack);
        }

        int write(int address, const char* data, int length, bool repeated = false) {
            return i2c_write(&_i2c, address, data, length, !repeated) != length;
        }

        int write(int data) {
            return i2c_byte_write(&_i2c, data);
        }

        void start(void) {
            i2c_start(&_i2c);
        }

        void stop(void) {
            i2c_stop(&_i2c);
        }

        /* mbino restriction: no virtual lock methods
        virtual void lock();
        virtual void unlock();
        */

#if DEVICE_I2C_ASYNCH
#error "Asynchronous I2C operation is not supported."
#endif

    protected:
        /* TODO: acquire() support?
        void aquire();
        */
    };

}

#endif

#endif
