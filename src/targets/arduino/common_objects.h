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
#ifndef MBINO_COMMON_OBJECTS_H
#define MBINO_COMMON_OBJECTS_H

#include <stdbool.h>
#include <stdint.h>

// forward declarations of Arduino C++ classes
struct Stream;
struct SPIClass;
struct TwoWire;

struct analogin_s {
    uint8_t pin;
};

struct serial_stream_interface_s {
    void (*begin)(struct Stream *obj, long baud, uint8_t cs, uint8_t pm, uint8_t sb);
    void (*end)(struct Stream *obj);
};

typedef struct serial_stream_interface_s serial_stream_interface_t;

struct serial_s {
    const struct serial_stream_interface_s *interface;
    struct Stream *stream;
    intptr_t irq_id;
    long baudrate;
    unsigned cs : 2;  // data_bits - 5
    unsigned pm : 3;  // parity
    unsigned sb : 1;  // stop_bits - 1
    bool initialized;
};

struct spi_s {
    struct SPIClass *spi;
    uint32_t clock;
    uint8_t bits;
    uint8_t mode;
    // cannot use SPISettings class in C interface
    union {
        int i;
        long l;
        char c[6];  // sizeof(SPISettings) on SAM processors
    } settings;
};

struct i2c_s {
    struct TwoWire* wire;
};

#endif
