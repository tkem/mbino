/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2018 Thomas Kemmer
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
#ifndef MBINO_ERROR_H
#define MBINO_ERROR_H

#include "platform/mbed_error.h"

#ifdef __WITH_AVRLIBC__
#include <avr/pgmspace.h>
#define MBINO_ERROR_STR(s) PSTR(s)
#else
#define MBINO_ERROR_STR(s) (s)
#endif

#ifdef NDEBUG
#define MBINO_ERROR(error_status, error_msg)    \
    mbino_error(error_status, NULL, 0, NULL, 0)
#define MBINO_ERROR1(error_status, error_msg, error_value) \
    mbino_error(error_status, NULL, error_value, NULL, 0)
#elif MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
#define MBINO_ERROR(error_status, MBINO_ERROR_STR(error_msg), 0) \
    mbino_error(error_status, MBINO_ERROR_STR(error_msg), 0, MBINO_ERROR_STR(MBED_FILENAME), __LINE__)
#define MBINO_ERROR1(error_status, MBINO_ERROR_STR(error_msg), error_value) \
    mbino_error(error_status, MBINO_ERROR_STR(error_msg), error_value, MBINO_ERROR_STR(MBED_FILENAME), __LINE__)
#else
#define MBINO_ERROR(error_status, error_msg) \
    mbino_error(error_status, MBINO_ERROR_STR(error_msg), 0, NULL, 0)
#define MBINO_ERROR1(error_status, error_msg, error_value) \
    mbino_error(error_status, MBINO_ERROR_STR(error_msg), error_value, NULL, 0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

void mbino_error(mbed_error_status_t status, const char *msg, unsigned value, const char *filename, int line_number);

#ifdef __cplusplus
}
#endif

#endif
