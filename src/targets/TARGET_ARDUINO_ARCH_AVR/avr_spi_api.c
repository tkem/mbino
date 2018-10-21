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
#ifdef ARDUINO_ARCH_AVR

#include "hal/spi_api.h"

#if DEVICE_SPI

#include <Arduino.h>

#include "platform/mbino_error.h"

#define SPI_INVARG MBED_MAKE_ERROR(MBED_MODULE_DRIVER_SPI, MBED_ERROR_CODE_INVALID_ARGUMENT)

#define SPCR_MODE(x) ((((x) & 2) << CPOL) | (((x) & 1) << CPHA))
#define SPCR_SPR(x) ((((x) & 2) << SPR1) | (((x) & 1) << SPR0))
#define SPSR_SPR(x) (((x) >> 2) << SPI2X)

/* Table 23-5. Relationship between SCK and Oscillator Frequency
 * SPI2X SPR01 SPR00 SCK
 *     0     0     0 fosc/4
 *     0     0     1 fosc/16
 *     0     1     0 fosc/64
 *     0     1     1 fosc/128
 *     1     0     0 fosc/2
 *     1     0     1 fosc/8
 *     1     1     0 fosc/32
 *     1     1     1 fosc/64
 */
static int spi_clock_rate(long f)
{
    if (f >= F_CPU / 2) {
        return 0x4;
    } else if (f >= F_CPU / 4) {
        return 0x0;
    } else if (f >= F_CPU / 8) {
        return 0x5;
    } else if (f >= F_CPU / 16) {
        return 0x1;
    } else if (f >= F_CPU / 32) {
        return 0x6;
    } else if (f >= F_CPU / 64) {
        return 0x2;
    } else {
        return 0x3;
    }
}

static void spi_pinout(PinName pin)
{
    uint8_t port = digitalPinToPort(pin);
    uint8_t mask = digitalPinToBitMask(pin);
    volatile uint8_t *reg = portModeRegister(port);
    uint8_t sreg = SREG;
    cli();
    *reg |= mask;
    SREG = sreg;
}

void spi_init(spi_t *obj, PinName mosi, PinName miso, PinName sclk, PinName ssel)
{
    if (mosi != SPI_MOSI || miso != SPI_MISO || sclk != SPI_SCK) {
        MBINO_ERROR(SPI_INVARG, "SPI pin mapping failed");
    } else if (ssel != NC && ssel != SPI_SS) {
        MBINO_ERROR(SPI_INVARG, "SPI SSEL pin mapping failed");
    } else {
        // default is master, 8 bits, mode 0, and a clock frequency of 1MHz
        obj->spr = spi_clock_rate(1000000L);
        // if SS becomes a low input in master mode, SPI switches to slave mode
        spi_pinout(SPI_SS);
        SPCR = (1 << SPE) | (1 << MSTR) | SPCR_MODE(0) | SPCR_SPR(obj->spr);
        // Arduino SPI library sets this *after* enabling SPI to avoid
        // accidentally clocking in a single bit, since the lines go
        // directly from "input" to SPI control
        spi_pinout(SPI_MOSI);
        spi_pinout(SPI_SCK);
        // AVR151: clear SPI interrupt flag by reading SPSR and SPDR
        (void)SPSR;
        (void)SPDR;
    }
}

void spi_free(spi_t *obj)
{
    SPCR = 0;
}

void spi_format(spi_t *obj, int bits, int mode, int slave)
{
    // note: only 8 bits are currently supported
    if (slave) {
        spi_pinout(SPI_MISO);
        // in slave mode SPR0 and SPR1 are not used
        SPCR = (1 << SPE) | SPCR_MODE(mode);
    } else {
        SPCR = (1 << SPE) | (1 << MSTR) | SPCR_MODE(mode) | SPCR_SPR(obj->spr);
    }
    // AVR151: clear SPI interrupt flag by reading SPSR and SPDR
    (void)SPSR;
    (void)SPDR;
}

void spi_frequency(spi_t *obj, long hz)
{
    obj->spr = spi_clock_rate(hz);
    uint8_t sreg = SREG;
    cli();
    SPCR = (SPCR & ~SPCR_SPR(~0)) | SPCR_SPR(obj->spr);
    SPSR = SPSR_SPR(obj->spr);
    SREG = sreg;
}

int spi_master_write(spi_t *obj, int value)
{
    SPDR = value;
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

int spi_master_block_write(spi_t *obj, const char *tx_buffer, int tx_length, char *rx_buffer, int rx_length, char write_fill)
{
    int n = tx_length < rx_length ? tx_length : rx_length;
    for (int i = 0; i != n; ++i) {
        rx_buffer[i] = spi_master_write(obj, tx_buffer[i]);
    }
    while (tx_length > n) {
        spi_master_write(obj, tx_buffer[n++]);
    }
    while (rx_length > n) {
        rx_buffer[n++] = spi_master_write(obj, write_fill);
    }
    return n;
}

int spi_slave_receive(spi_t *obj)
{
    return (SPSR & (1 << SPIF));
}

int spi_slave_read(spi_t *obj)
{
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

void spi_slave_write(spi_t *obj, int value)
{
    SPDR = value;
}

int spi_busy(spi_t *obj)
{
    return 0;  // TODO: proper implementation on AVR?
}

uint8_t spi_get_module(spi_t *obj)
{
    return 0;
}

#endif

#endif
