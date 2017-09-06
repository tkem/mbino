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
#include "spi_api.h"

#ifdef DEVICE_SPI

#include <SPI.h>

#include <string.h>

// TBD: extern "C"?
namespace mbino {

    static void spi_init_settings(spi_t* obj)
    {
        static const uint8_t mode[] = {SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3};
        // Arduino does not support bits, while mbed does not have bit order...
        obj->settings = SPISettings(obj->clock, MSBFIRST, mode[obj->mode & 0x3]);
    }

    void spi_init(spi_t* obj, PinName mosi, PinName miso, PinName sclk)
    {
        // TODO: handle pin names (software SPI?)
        SPI.begin();  // Arduino SPI has its own init counter
        obj->clock = 1000000;
        obj->bits = 8;
        obj->mode = 0;
        spi_init_settings(obj);
    }

    void spi_free(spi_t* obj)
    {
        SPI.end();
    }

    void spi_format(spi_t* obj, uint8_t bits, uint8_t mode)
    {
        obj->bits = bits;
        obj->mode = mode;
        spi_init_settings(obj);
    }

    void spi_frequency(spi_t* obj, long hz)
    {
        obj->clock = hz;
        spi_init_settings(obj);
    }

    int spi_master_write(spi_t* obj, int value)
    {
        int data;
        SPI.beginTransaction(obj->settings);
        if (obj->bits > 8) {
            data = SPI.transfer16(value);
        } else {
            data = SPI.transfer(value);
        }
        SPI.endTransaction();
        return data;
    }

    void spi_master_block_write(
        spi_t* obj,
        const char* tx_buffer, int tx_length,
        char* rx_buffer, int rx_length,
        char write_fill)
    {
        int n = tx_length < rx_length ? tx_length : rx_length;
        memcpy(rx_buffer, tx_buffer, n);
        memset(rx_buffer + n, write_fill, rx_length - n);

        SPI.beginTransaction(obj->settings);
        if (obj->bits > 8) {
            for (int i = 0; i < rx_length; ++i) {
                rx_buffer[i] = SPI.transfer16(rx_buffer[i]);
            }
            while (tx_length > n) {
                SPI.transfer16(tx_buffer[n++]);
            }
        } else {
            if (rx_length > 0) {
                SPI.transfer(rx_buffer, rx_length);
            }
            while (tx_length > n) {
                SPI.transfer(tx_buffer[n++]);
            }
        }
        SPI.endTransaction();
    }
}

#endif
