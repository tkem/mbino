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
#ifndef MBINO_SPI_API_H
#define MBINO_SPI_API_H

#include "platform/platform.h"

#ifdef DEVICE_SPI

// TODO: do we really have to include SPI.h here?
#include <SPI.h>

// TBD: extern "C"?
namespace mbino {

    static const char SPI_FILL_CHAR = 0xFF;

    struct spi_t {
        uint32_t clock;
        uint8_t bits;
        uint8_t mode;
        SPISettings settings;  // TODO: anonymous struct?
        bool initialized;
    };

    void spi_init(spi_t* obj, PinName mosi, PinName miso, PinName sclk);

    void spi_free(spi_t* obj);

    void spi_format(spi_t* obj, uint8_t bits, uint8_t mode);

    void spi_frequency(spi_t* obj, long hz);

    int spi_master_write(spi_t* obj, int value);

    void spi_master_block_write(
        spi_t* obj,
        const char* tx_buffer, int tx_length,
        char* rx_buffer, int rx_length,
        char write_fill);

}

#endif

#endif
