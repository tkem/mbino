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
#include "hal/spi_api.h"

#ifdef DEVICE_SPI

#include "platform/mbed_assert.h"
#include "platform/mbed_error.h"

#include <SPI.h>

#include <string.h>

inline void* operator new(size_t, decltype(spi_t::settings)* p) {
    MBED_STATIC_ASSERT(sizeof(SPISettings) <= sizeof *p,
                       "Insufficient space for SPISettings object");
    return p;
}

static void spi_init_settings(spi_t* obj)
{
    // Arduino does not support bits, while mbed does not have bit order...
    static const uint8_t mode[] = {SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3};
    new(&obj->settings) SPISettings(obj->clock, MSBFIRST, mode[obj->mode & 0x3]);
}

static const SPISettings& spi_get_settings(spi_t* obj)
{
    return *reinterpret_cast<SPISettings*>(&obj->settings);
}

void spi_init(spi_t* obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    if (ssel != NC) {
        error("Hardware SSEL not supported");
    }
    if (mosi == PIN_SPI_MOSI && miso == PIN_SPI_MISO && sclk == PIN_SPI_SCK) {
        obj->spi = &SPI;
#if SPI_INTERFACES_COUNT > 1
    } else if (mosi == PIN_SPI1_MOSI && miso == PIN_SPI1_MISO && sclk == PIN_SPI1_SCK) {
        obj->spi = &SPI1;
#endif
#if SPI_INTERFACES_COUNT > 2
    } else if (mosi == PIN_SPI2_MOSI && miso == PIN_SPI2_MISO && sclk == PIN_SPI2_SCK) {
        obj->spi = &SPI2;
#endif
#if SPI_INTERFACES_COUNT > 3
    } else if (mosi == PIN_SPI3_MOSI && miso == PIN_SPI3_MISO && sclk == PIN_SPI3_SCK) {
        obj->spi = &SPI3;
#endif
#if SPI_INTERFACES_COUNT > 4
    } else if (mosi == PIN_SPI4_MOSI && miso == PIN_SPI4_MISO && sclk == PIN_SPI4_SCK) {
        obj->spi = &SPI4;
#endif
#if SPI_INTERFACES_COUNT > 5
    } else if (mosi == PIN_SPI5_MOSI && miso == PIN_SPI5_MISO && sclk == PIN_SPI5_SCK) {
        obj->spi = &SPI5;
#endif
    } else {
        error("SPI pin mapping failed");
    }
    obj->clock = 1000000;
    obj->bits = 8;
    obj->mode = 0;
    spi_init_settings(obj);
    obj->spi->begin();  // Arduino SPI has its own init counter
}

void spi_free(spi_t* obj)
{
    obj->spi->end();
}

void spi_format(spi_t* obj, int bits, int mode, int slave)
{
    if (slave) {
        error("SPI slave mode not supported");
    }
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
    obj->spi->beginTransaction(spi_get_settings(obj));
    if (obj->bits > 8) {
        data = obj->spi->transfer16(value);
    } else {
        data = obj->spi->transfer(value);
    }
    obj->spi->endTransaction();
    return data;
}

int spi_master_block_write(
    spi_t* obj,
    const char* tx_buffer, int tx_length,
    char* rx_buffer, int rx_length,
    char write_fill)
{
    int n = tx_length < rx_length ? tx_length : rx_length;
    memcpy(rx_buffer, tx_buffer, n);
    memset(rx_buffer + n, write_fill, rx_length - n);

    obj->spi->beginTransaction(spi_get_settings(obj));
    if (obj->bits > 8) {
        for (int i = 0; i < rx_length; ++i) {
            rx_buffer[i] = obj->spi->transfer16(rx_buffer[i]);
        }
        while (tx_length > n) {
            obj->spi->transfer16(tx_buffer[n++]);
        }
    } else {
        if (rx_length > 0) {
            obj->spi->transfer(rx_buffer, rx_length);
        }
        while (tx_length > n) {
            obj->spi->transfer(tx_buffer[n++]);
        }
    }
    obj->spi->endTransaction();
    return tx_length > rx_length ? tx_length : rx_length;
}

#endif
