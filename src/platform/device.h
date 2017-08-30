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
#ifndef MBINO_DEVICE_H
#define MBINO_DEVICE_H

#include <pins_arduino.h>

// not defined for gemma?
#ifndef NUM_DIGITAL_PINS
#define NUM_DIGITAL_PINS 6
#endif

#ifdef LED_BUILTIN
#define PIN_LED1 LED_BUILTIN
#endif

#if defined(SERIAL_PORT_MONITOR) || defined(SERIAL_PORT_HARDWARE)
#define DEVICE_SERIAL 1

// see https://www.arduino.cc/en/Reference/Serial
#ifdef SERIAL_PORT_HARDWARE
#define PIN_SERIAL_PORT_HARDWARE_TX 1
#define PIN_SERIAL_PORT_HARDWARE_RX 0
#endif
#ifdef SERIAL_PORT_HARDWARE1
#define PIN_SERIAL_PORT_HARDWARE1_TX 18
#define PIN_SERIAL_PORT_HARDWARE1_RX 19
#endif
#ifdef SERIAL_PORT_HARDWARE2
#define PIN_SERIAL_PORT_HARDWARE2_TX 16
#define PIN_SERIAL_PORT_HARDWARE2_RX 17
#endif
#ifdef SERIAL_PORT_HARDWARE3
#define PIN_SERIAL_PORT_HARDWARE3_TX 14
#define PIN_SERIAL_PORT_HARDWARE3_RX 15
#endif

#endif

#if defined(PIN_WIRE_SCL) && defined(PIN_WIRE_SDA)
#define DEVICE_I2C 1
#define PIN_I2C_SCL PIN_WIRE_SCL
#define PIN_I2C_SDA PIN_WIRE_SDA
#endif

#if defined(PIN_SPI_MISO) && defined(PIN_SPI_MOSI) && defined(PIN_SPI_SCK)
#define DEVICE_SPI 1
#endif

#endif
