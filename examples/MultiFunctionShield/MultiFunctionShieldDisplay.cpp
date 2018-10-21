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
#include "MultiFunctionShieldDisplay.h"

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>

const uint8_t digit_seg[] = {
    0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f
};

const uint8_t alpha_seg[] = {
    0x77, 0x7c, 0x58, 0x5e, 0x79, 0x71, 0x6f, 0x74, 0x30, 0x0e,
    0x49, 0x38, 0x49, 0x54, 0x5c, 0x73, 0x67, 0x50, 0x6d, 0x78,
    0x1c, 0x49, 0x49, 0x49, 0x6e, 0x49,
};

const uint8_t space_seg = 0x00;
const uint8_t minus_seg = 0x40;
const uint8_t ldash_seg = 0x08;
const uint8_t error_seg = 0x49;
const uint8_t punct_seg = 0x80;

MultiFunctionShieldDisplay::MultiFunctionShieldDisplay(
    PinName latch, PinName shift, PinName data, bool inverted)
    : latch_pin(latch), shift_pin(shift), data_pin(data)
{
    invmask = inverted ? 0xff : 0;
    write(0, space_seg);
    clear();
}

void MultiFunctionShieldDisplay::start(int update_interval_us)
{
    ticker.attach_us(callback(this, &MultiFunctionShieldDisplay::update), update_interval_us);
}

void MultiFunctionShieldDisplay::stop()
{
    ticker.detach();
}

void MultiFunctionShieldDisplay::printf(const char* format, ...)
{
    char buffer[8];  // larger buffer to allow for punctuation
    uint8_t data[4] = { space_seg, space_seg, space_seg, space_seg, };

    va_list arg;
    va_start(arg, format);
    int n = vsnprintf(buffer, sizeof buffer, format, arg);
    va_end(arg);

    for (int i = 0, j = 0; i < n && j < 4 && buffer[i]; ++i) {
        char c = tolower(buffer[i]);
        if (isdigit(c)) {
            data[j++] = digit_seg[c - '0'];
        } else if (isalpha(c)) {
            data[j++] = alpha_seg[c - 'a'];
        } else if (isspace(c)) {
            data[j++] = space_seg;
        } else if (c == '-') {
            data[j++] = minus_seg;
        } else if (c == '_') {
            data[j++] = ldash_seg;
        } else if (c == '.' && j != 0 && !(data[j - 1] & punct_seg)) {
            data[j - 1] |= punct_seg;
        } else if (c == '.') {
            data[j++] = punct_seg;
        } else {
            data[j++] = error_seg;
        }
    }
    set(data);
}

void MultiFunctionShieldDisplay::set(const uint8_t segments[4])
{
    memcpy(values, segments, sizeof values);
}

void MultiFunctionShieldDisplay::clear()
{
    memset(values, 0xff, sizeof values);
}

void MultiFunctionShieldDisplay::update()
{
    write(index, values[index]);
    if (++index >= 4) {
        index = 0;
    }
}

void MultiFunctionShieldDisplay::write(int index, uint8_t value)
{
    // MC74HC595A is positive-edge triggered; max. clock frequency at
    // 3.0 V VCC is 10 MHz and min. setup time is 50 ns, so we might
    // get away without wait_us() for now...
    uint8_t segment = 0xf0 | (1 << index);
    value ^= invmask;
    for (int i = 0; i != 8; ++i) {
        data_pin = value & 0x80;
        shift_pin = 0;
        shift_pin = 1;
        value <<= 1;
    }
    for (int i = 0; i != 8; ++i) {
        data_pin = segment & 0x80;
        shift_pin = 0;
        shift_pin = 1;
        segment <<= 1;
    }
    latch_pin = 0;
    latch_pin = 1;
}
