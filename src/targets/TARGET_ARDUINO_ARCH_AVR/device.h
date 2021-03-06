/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
#ifndef MBINO_AVR_DEVICE_H
#define MBINO_AVR_DEVICE_H

#include "objects.h"

#define DEVICE_ANALOGIN 1
#define DEVICE_INTERRUPTIN 1
#define DEVICE_PORTIN 1
#define DEVICE_PORTINOUT 1
#define DEVICE_PORTOUT 1
#define DEVICE_PWMOUT 1

#if defined(SERIAL_PORT_HARDWARE) || defined(SERIAL_PORT_HARDWARE1)
#define DEVICE_SERIAL 1
#endif

#if defined(SERIAL_PORT_MONITOR) && !defined(SERIAL_PORT_USBVIRTUAL)
#define DEVICE_STDIO_MESSAGES 1
#endif

#if defined(PIN_SPI_MISO) && defined(PIN_SPI_MOSI) && defined(PIN_SPI_SCK)
#define DEVICE_SPI 1
#define DEVICE_SPISLAVE 1
#endif

#if defined(PIN_WIRE_SCL) && defined(PIN_WIRE_SDA)
#define DEVICE_I2C 1
#define DEVICE_I2CSLAVE 1
#endif

#endif
