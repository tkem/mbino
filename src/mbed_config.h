/*
 * mbed SDK
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __MBED_CONFIG_DATA__
#define __MBED_CONFIG_DATA__

#ifndef MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE
#define MBED_CONF_DRIVERS_UART_SERIAL_RXBUF_SIZE                        16
#endif
#ifndef MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE
#define MBED_CONF_DRIVERS_UART_SERIAL_TXBUF_SIZE                        16
#endif

#ifndef MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE
#define MBED_CONF_PLATFORM_DEFAULT_SERIAL_BAUD_RATE                     9600
#endif
#ifndef MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO
#define MBED_CONF_PLATFORM_ERROR_ALL_THREADS_INFO                       0
#endif
#ifndef MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
#define MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED               0
#endif
#ifndef MBED_CONF_PLATFORM_ERROR_HIST_ENABLED
#define MBED_CONF_PLATFORM_ERROR_HIST_ENABLED                           0
#endif
#ifndef MBED_CONF_PLATFORM_ERROR_HIST_SIZE
#define MBED_CONF_PLATFORM_ERROR_HIST_SIZE                              4
#endif
#ifndef MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR
#define MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR                     0
#endif
#ifndef MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN
#define MBED_CONF_PLATFORM_MAX_ERROR_FILENAME_LEN                       16
#endif
#ifndef MBED_CONF_PLATFORM_POLL_USE_LOWPOWER_TIMER
#define MBED_CONF_PLATFORM_POLL_USE_LOWPOWER_TIMER                      0
#endif
#ifndef MBED_CONF_PLATFORM_STDIO_BAUD_RATE
#define MBED_CONF_PLATFORM_STDIO_BAUD_RATE                              9600
#endif
#ifndef MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL
#define MBED_CONF_PLATFORM_STDIO_BUFFERED_SERIAL                        0
#endif
#ifndef MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES
#define MBED_CONF_PLATFORM_STDIO_CONVERT_NEWLINES                       0
#endif
#ifndef MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES
#define MBED_CONF_PLATFORM_STDIO_CONVERT_TTY_NEWLINES                   0
#endif
#ifndef MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT
#define MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT                          1
#endif

// TODO: MBED_CONF_TARGET_STDIO_UART_TX
// TODO: MBED_CONF_TARGET_STDIO_UART_RX

#endif
