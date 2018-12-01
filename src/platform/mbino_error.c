/* mbino - mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "platform.h"
#include "mbed_interface.h"
#include "mbed_toolchain.h"
#include "mbino_error.h"

// TODO: rest of error API, history?

static bool error_in_progress = false;

MBED_WEAK void error(const char *format, ...)
{
    if (!error_in_progress) {
        error_in_progress = true;
        // TODO: report "Fatal Run-time error"?
#ifndef NDEBUG
        va_list arg;
        va_start(arg, format);
        mbed_error_vfprintf(format, arg);
        va_end(arg);
#endif
        mbed_die();
    }
}

#ifdef __WITH_AVRLIBC__

MBED_WEAK mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number)
{
    if (!error_in_progress) {
        error_in_progress = true;
#ifndef NDEBUG
        unsigned error_code = MBED_GET_ERROR_CODE(error_status);
        unsigned error_module = MBED_GET_ERROR_MODULE(error_status);

        mbed_error_printf("%S%lX%S%d%S%d",
                          PSTR("\n\n++ MbedOS Error Info ++\nError Status: 0x"),
                          error_status,
                          PSTR(" Code: "),
                          error_code,
                          PSTR(" Module: "),
                          error_module);
        if (error_msg) {
            mbed_error_printf("%S%s", PSTR("\nError Message: "), error_msg);
        }
#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
        if (filename && line_number) {
            mbed_error_printf("%S%s+%d", PSTR("\nFile:"), filename, line_number);
        }
#endif
        mbed_error_printf("%S%X%S", PSTR("\nError Value: 0x"), error_value, PSTR("\n-- MbedOS Error Info --\n"));
#endif
        mbed_die();
    }
    return MBED_ERROR_FAILED_OPERATION;
}

void mbino_error(mbed_error_status_t status, const char *msg, unsigned value, const char *filename, int line_number)
{
#ifndef NDEBUG
    unsigned error_code = MBED_GET_ERROR_CODE(status);
    unsigned error_module = MBED_GET_ERROR_MODULE(status);

    mbed_error_printf("%S%lX%S%d%S%d",
                      PSTR("\n\n++ MbedOS Error Info ++\nError Status: 0x"),
                      status,
                      PSTR(" Code: "),
                      error_code,
                      PSTR(" Module: "),
                      error_module);
    if (msg) {
        mbed_error_printf("%S%S", PSTR("\nError Message: "), msg);
    }
#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
    if (filename && line_number) {
        mbed_error_printf("%S%S+%d", PSTR("\nFile:"), filename, line_number);
    }
#endif
    mbed_error_printf("%S%X%S", PSTR("\nError Value: 0x"), value, PSTR("\n-- MbedOS Error Info --\n"));
#endif
    mbed_die();
}

#else

MBED_WEAK mbed_error_status_t mbed_error(mbed_error_status_t error_status, const char *error_msg, unsigned int error_value, const char *filename, int line_number)
{
    if (!error_in_progress) {
        error_in_progress = true;
        mbino_error(error_status, error_msg, error_value, filename, line_number);
    }
    return MBED_ERROR_FAILED_OPERATION;
}

void mbino_error(mbed_error_status_t status, const char *msg, unsigned value, const char *filename, int line_number)
{
#ifndef NDEBUG
    unsigned error_code = MBED_GET_ERROR_CODE(status);
    unsigned error_module = MBED_GET_ERROR_MODULE(status);

    mbed_error_printf("\n\n++ MbedOS Error Info ++\nError Status: 0x%X Code: %d Module: %d",
                      status, error_code, error_module);
    if (msg) {
        mbed_error_printf("\nError Message: %s", msg);
    }
#if MBED_CONF_PLATFORM_ERROR_FILENAME_CAPTURE_ENABLED
    if (filename && line_number) {
        mbed_error_printf("\nFile:%s+%d", filename, line_number);
    }
#endif
    mbed_error_printf("\nError Value: 0x%X\n-- MbedOS Error Info --\n", value);
#endif
    mbed_die();
}

#endif
