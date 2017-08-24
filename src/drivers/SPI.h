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
#ifndef MBINO_SPI_H
#define MBINO_SPI_H

#include "platform/platform.h"

#if defined(DEVICE_SPI) || defined(DOXYGEN_ONLY)

#include "hal/spi_api.h"
#include "platform/NonCopyable.h"

namespace mbino {

    class SPI : private NonCopyable<SPI> {
        spi_t _spi;
        char _write_fill;

    public:

        SPI(PinName mosi, PinName miso, PinName sclk) : _write_fill(SPI_FILL_CHAR) {
            spi_init(&_spi, mosi, miso, sclk);
        }

        void format(int bits, int mode = 0) {
            spi_format(&_spi, bits, mode);
        }

        void frequency(long hz = 1000000) {
            spi_frequency(&_spi, hz);
        }

        int write(int value) {
            return spi_master_write(&_spi, value);
        }

        int write(const char* tx_buffer, int tx_length, char* rx_buffer, int rx_length) {
            spi_master_block_write(&_spi, tx_buffer, tx_length, rx_buffer, rx_length, _write_fill);
            return tx_length > rx_length ? tx_length : rx_length;
        }

        void set_default_write_value(char data) {
            _write_fill = data;
        }

        // no multithreading support
        void lock() {}
        void unlock() {}
    };

}

#endif

#endif
