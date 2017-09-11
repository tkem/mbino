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
#include "ticker_api.h"

#include <Arduino.h>

#include <stdint.h>

namespace mbino {

    static void schedule_interrupt(const ticker_data_t* ticker);

    static us_timestamp_t convert_timestamp(us_timestamp_t ref, uint32_t timestamp)
    {
        us_timestamp_t result = (ref & ~(us_timestamp_t(UINT32_MAX))) | timestamp;
        if (timestamp < uint32_t(ref)) {
            result += 1ULL << 32;
        }
        return result;
    }

    static void update_present_time(const ticker_data_t* ticker)
    {
        if (!ticker->queue->initialized) {
            ticker->interface->init();
            ticker->queue->initialized = true;
            schedule_interrupt(ticker);
        }
        ticker->queue->present_time = convert_timestamp(
            ticker->queue->present_time,
            ticker->interface->read()
            );
    }

    static void schedule_interrupt(const ticker_data_t* ticker)
    {
        uint32_t delta = ticker->interface->interrupt_timestamp_max_delta;
        update_present_time(ticker);
        if (ticker->queue->head) {
            us_timestamp_t present_time = ticker->queue->present_time;
            us_timestamp_t next_event_timestamp = ticker->queue->head->timestamp;
            if (next_event_timestamp < present_time) {
                delta = 0;
            } else if (next_event_timestamp - present_time < delta) {
                delta = next_event_timestamp - present_time;
            }
        }
        ticker->interface->set_interrupt(ticker->queue->present_time + delta);
    }

    void ticker_irq_handler(const ticker_data_t* ticker)
    {
        while (ticker->queue->head) {
            update_present_time(ticker);
            if (ticker->queue->head->timestamp <= ticker->queue->present_time) {
                ticker_event_t* p = ticker->queue->head;
                ticker->queue->head = ticker->queue->head->next;
                p->handler(p->id);
            } else {
                break;  // from loop
            }
        }
        schedule_interrupt(ticker);
    }

    void ticker_init_event(ticker_event_t* obj, ticker_event_handler handler, intptr_t id)
    {
        obj->handler = handler;
        obj->id = id;
    }

    void ticker_insert_event(const ticker_data_t* ticker, ticker_event_t* obj, timestamp_t timestamp)
    {
        uint8_t sreg = SREG;
        cli();
        update_present_time(ticker);
        us_timestamp_t absolute_timestamp = convert_timestamp(
            ticker->queue->present_time,
            timestamp);
        SREG = sreg;

        ticker_insert_event_us(ticker, obj, absolute_timestamp);
    }

    void ticker_insert_event_us(const ticker_data_t* ticker, ticker_event_t* obj, us_timestamp_t timestamp)
    {
        uint8_t sreg = SREG;
        cli();

        obj->timestamp = timestamp;

        ticker_event_t* p = ticker->queue->head;
        ticker_event_t* prev = 0;
        while (p && timestamp > p->timestamp) {
            prev = p;
            p = p->next;
        }
        obj->next = p;

        if (!prev) {
            ticker->queue->head = obj;
            schedule_interrupt(ticker);
        } else {
            prev->next = obj;
        }

        SREG = sreg;
    }

    void ticker_remove_event(const ticker_data_t* ticker, ticker_event_t* obj)
    {
        uint8_t sreg = SREG;
        cli();

        if (ticker->queue->head == obj) {
            ticker->queue->head = obj->next;
            schedule_interrupt(ticker);
        } else {
            for (ticker_event_t* p = ticker->queue->head; p; p = p->next) {
                if (p->next == obj) {
                    p->next = obj->next;
                    break;  // from loop
                }
            }
        }

        SREG = sreg;
    }

    timestamp_t ticker_read(const ticker_data_t* ticker)
    {
        return ticker->interface->read();
    }

    us_timestamp_t ticker_read_us(const ticker_data_t* ticker)
    {
        us_timestamp_t time;
        uint8_t sreg = SREG;
        cli();
        update_present_time(ticker);
        time = ticker->queue->present_time;
        SREG = sreg;
        return time;
    }
}
