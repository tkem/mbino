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
#ifndef MBINO_MULTI_FUNCTION_SHIELD_DISPLAY_H
#define MBINO_MULTI_FUNCTION_SHIELD_DISPLAY_H

#include "mbed.h"

class MultiFunctionShieldDisplay {
    Ticker ticker;
    DigitalOut latch_pin;
    DigitalOut shift_pin;
    DigitalOut data_pin;
    uint8_t values[4];
    uint8_t invmask;
    int index;

public:
    MultiFunctionShieldDisplay(PinName latch, PinName shift, PinName data, bool inverted = true);

    void start(int update_interval_us = 2048);

    void stop();

    void printf(const char* format, ...);

    void set(const uint8_t segments[4]);

    void clear();

private:
    void write(int index, uint8_t value);
    void update();
};


#endif
