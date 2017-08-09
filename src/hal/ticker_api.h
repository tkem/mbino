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
#ifndef MBINO_TICKER_API_H
#define MBINO_TICKER_API_H

#include <stdint.h>

// TBD: extern "C"?
namespace mbino {

    typedef uint32_t timestamp_t;

    typedef uint64_t us_timestamp_t;

    typedef void (*ticker_event_handler)(intptr_t id);

    struct ticker_event_t {
        us_timestamp_t timestamp;
        ticker_event_t* next;
        ticker_event_handler handler;
        intptr_t id;
    };

    struct ticker_interface_t {
        void (*init)();
        uint32_t (*read)();
        void (*set_interrupt)(timestamp_t timestamp);
        uint32_t interrupt_timestamp_max_delta;
    };

    struct ticker_event_queue_t {
        us_timestamp_t present_time;
        ticker_event_t* head;
        bool initialized;
    };

    struct ticker_data_t {
        const ticker_interface_t* interface;
        ticker_event_queue_t* queue;
    };

    void ticker_irq_handler(const ticker_data_t* ticker);

    inline void ticker_init_event(ticker_event_t* obj, ticker_event_handler handler, intptr_t id) {
        obj->handler = handler;
        obj->id = id;
    }

    void ticker_insert_event(const ticker_data_t* ticker, ticker_event_t* obj, timestamp_t timestamp);

    void ticker_insert_event_us(const ticker_data_t* const ticker, ticker_event_t* obj, us_timestamp_t timestamp);

    void ticker_remove_event(const ticker_data_t* ticker, ticker_event_t* obj);

    inline timestamp_t ticker_read(const ticker_data_t* const ticker) {
        return ticker->interface->read();
    }

    us_timestamp_t ticker_read_us(const ticker_data_t* ticker);
}

#endif
