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
#ifndef MBINO_OBJECTS_H
#define MBINO_OBJECTS_H

#include "PinNames.h"
#include "tickers.h"

#include <pins_arduino.h>

#include <stdbool.h>

#if defined(ARDUINO_ARCH_AVR)
#include "avr/objects.h"
#else
#error “This library only supports boards with an AVR processor.”
#endif

#define DEVICE_ANALOGIN 1
#define DEVICE_INTERRUPTIN 1

#if defined(SERIAL_PORT_MONITOR) || defined(SERIAL_PORT_HARDWARE)
#define DEVICE_SERIAL 1
#endif

#if defined(PIN_SPI_MISO) && defined(PIN_SPI_MOSI) && defined(PIN_SPI_SCK)
#define DEVICE_SPI 1
#endif

#if defined(PIN_WIRE_SCL) && defined(PIN_WIRE_SDA)
#define DEVICE_I2C 1
#endif

struct analogin_s {
    uint8_t pin;
};

struct gpio_irq_s {
    gpio_t gpio;
    intptr_t id;
    uint8_t irq;
    uint8_t events;
    bool enabled;
};

#if DEVICE_SERIAL
struct Stream; // forward declaration of Arduino Stream class

typedef struct {
    void (*begin)(struct Stream *obj, long baud, int data, int parity, int stop);
    void (*end)(struct Stream *obj);
} serial_stream_interface_t;

struct serial_s {
    const serial_stream_interface_t *interface;
    struct Stream *stream;
    long baudrate;
    // TODO: bit fields or compact single-byte encoding
    uint8_t data;
    uint8_t parity;
    uint8_t stop;
    bool initialized;
};
#endif

#if DEVICE_SPI
struct spi_s {
    uint32_t clock;
    uint8_t bits;
    uint8_t mode;
    // cannot use SPISettings class in C interface
    union {
        int i;
        char c[6];  // needed on SAM processors
    } settings;
};
#endif

#if DEVICE_I2C
struct i2c_s {};
// HACK to avoid pulling in the Wire library stuff...
#if defined(WIRE_HAS_END) && defined(__cplusplus)
// assume Wire.h was already included and the global Wire object is available
extern "C" {
    typedef struct i2c_s i2c_t;

    inline void i2c_init(i2c_t* obj, PinName sda, PinName scl) {
        if (sda == I2C_SDA && scl == I2C_SCL) {
            // FIXME: on first call? Prevent multiple initialization?
            Wire.begin();
        }
        // TODO: error handling?
    }

    inline void i2c_frequency(i2c_t* obj, long hz) {
        Wire.setClock(hz);
    }

    inline int i2c_read(i2c_t* obj, int address, char* data, int length, int stop) {
        // FIXME: nread > length?
        uint8_t nread = Wire.requestFrom(address >> 1, length, stop);
        int n = 0;
        // FIXME: available() < nread?
        while (n != nread && Wire.available()) {
            data[n++] = Wire.read();
        }
        return n;
    }

    inline int i2c_write(i2c_t* obj, int address, const char* data, int length, int stop) {
        Wire.beginTransmission(address >> 1);
        int n = Wire.write(data, length);
        // FIXME: error mapping?
        switch (Wire.endTransmission(stop)) {
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
}

#endif
#endif

#endif
