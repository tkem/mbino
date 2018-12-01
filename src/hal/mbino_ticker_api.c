/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
 *
 * mbed Microcontroller Library
 * Copyright (c) 2015 ARM Limited
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
#include <limits.h>
#include <stdint.h>

#include "ticker_api.h"
#include "platform/mbed_assert.h"
#include "platform/mbed_critical.h"

#define TICKER_FREQUENCY    1000000
#define TICKER_BITS         32
#define TICKER_MAX_DELTA    (0x7UL << (TICKER_BITS - 4))
#define TICKER_MAX_DELTA_US ((us_timestamp_t)TICKER_MAX_DELTA)

static void schedule_interrupt(const ticker_data_t *const ticker);
static void update_present_time(const ticker_data_t *const ticker);

static void default_handler(uint32_t id)
{
    (void)id;
}

static void initialize(const ticker_data_t *ticker)
{
    if (!ticker->queue->initialized) {
        /* for now, only 32-bit 1MHz tickers are supported
#ifndef NDEBUG
        // mbino requires 32-bit 1MHz tickers
        const ticker_info_t *info = ticker->interface->get_info();
        MBED_ASSERT(info->frequency == TICKER_FREQUENCY);
        MBED_ASSERT(info->bits == TICKER_BITS);
#endif
        */
        ticker->interface->init();
        ticker->queue->event_handler = default_handler;
        ticker->queue->head = NULL;
        ticker->queue->tick_last_read = ticker->interface->read();
        ticker->queue->present_time = 0;
        ticker->queue->dispatching = false;
        ticker->queue->initialized = true;
        // schedule_interrupt() also updates present time
        schedule_interrupt(ticker);
    }
}

static void set_handler(const ticker_data_t *const ticker, ticker_event_handler handler)
{
    ticker->queue->event_handler = handler ? handler : default_handler;
}

static void update_present_time(const ticker_data_t *ticker)
{
    ticker_event_queue_t *queue = ticker->queue;
    uint32_t ticker_time = ticker->interface->read();
    uint32_t elapsed_ticks = ticker_time - queue->tick_last_read;
    queue->tick_last_read = ticker_time;
    // mbino requires 32-bit 1MHz tickers, so 1 tick equals 1 us
    queue->present_time += elapsed_ticks;
}

static void schedule_interrupt(const ticker_data_t *ticker)
{
    ticker_event_queue_t *queue = ticker->queue;
    // don't schedule interrupts until we're finished dispatching
    if (queue->dispatching) {
        return;
    }

    update_present_time(ticker);

    if (queue->head) {
        us_timestamp_t match_time = queue->head->timestamp;
        us_timestamp_t present_time = queue->present_time;
        us_timestamp_t delta_us = match_time - present_time;

        if (match_time <= present_time) {
            ticker->interface->fire_interrupt();
        } else if (delta_us > TICKER_MAX_DELTA_US) {
            ticker->interface->set_interrupt(queue->tick_last_read + TICKER_MAX_DELTA);
        } else {
            // for now, checking if match_time has expired in the mean
            // time is subject to the target's interrupt handler
            ticker->interface->set_interrupt(queue->tick_last_read + (timestamp_t)delta_us);
        }
    } else {
        ticker->interface->set_interrupt(queue->tick_last_read + TICKER_MAX_DELTA);
    }
}

void ticker_set_handler(const ticker_data_t *const ticker, ticker_event_handler handler)
{
    initialize(ticker);

    core_util_critical_section_enter();
    set_handler(ticker, handler);
    core_util_critical_section_exit();
}

void ticker_irq_handler(const ticker_data_t *const ticker)
{
    ticker_event_queue_t *queue = ticker->queue;
#ifndef ARDUINO_ARCH_AVR
    // IRQs (normally) won't be interrupted on AVR
    core_util_critical_section_enter();
    // no need to clear interrupts on AVR
    ticker->interface->clear_interrupt();
#endif
    ticker->queue->dispatching = true;

    // update present time only once per interrupt
    update_present_time(ticker);

    while (queue->head && queue->head->timestamp <= queue->present_time) {
        ticker_event_t *p = queue->head;
        queue->head = p->next;
        (*queue->event_handler)(p->id); // NOTE: the handler can set new events
    }

    ticker->queue->dispatching = false;

    schedule_interrupt(ticker);

#ifndef ARDUINO_ARCH_AVR
    core_util_critical_section_exit();
#endif
}

void ticker_insert_event(const ticker_data_t *const ticker, ticker_event_t *obj, timestamp_t timestamp, uint32_t id)
{
    us_timestamp_t absolute_timestamp = ticker_read_us(ticker);
    bool overflow = timestamp < (timestamp_t)absolute_timestamp;
    absolute_timestamp &= ~((us_timestamp_t)UINT32_MAX);
    absolute_timestamp |= timestamp;
    if (overflow) {
        absolute_timestamp += (1ULL << 32);
    }
    ticker_insert_event_us(ticker, obj, absolute_timestamp, id);
}

void ticker_insert_event_us(const ticker_data_t *const ticker, ticker_event_t *obj, us_timestamp_t timestamp, uint32_t id)
{
    ticker_event_queue_t *queue = ticker->queue;

    core_util_critical_section_enter();

    obj->timestamp = timestamp;
    obj->id = id;

    ticker_event_t *prev = NULL;
    ticker_event_t *p = queue->head;
    while (p && timestamp >= p->timestamp) {
        prev = p;
        p = p->next;
    }
    obj->next = p;

    if (!prev) {
        queue->head = obj;
        schedule_interrupt(ticker);
    } else {
        prev->next = obj;
    }

    core_util_critical_section_exit();
}

void ticker_remove_event(const ticker_data_t *const ticker, ticker_event_t *obj)
{
    ticker_event_queue_t *queue = ticker->queue;

    core_util_critical_section_enter();

    if (queue->head == obj) {
        queue->head = obj->next;
        schedule_interrupt(ticker);
    } else {
        ticker_event_t* p = queue->head;
        while (p != NULL) {
            if (p->next == obj) {
                p->next = obj->next;
                break;
            }
            p = p->next;
        }
    }

    core_util_critical_section_exit();
}

timestamp_t ticker_read(const ticker_data_t *const ticker)
{
    // assuming all mbino tickers can be read without initialization
    return ticker->interface->read();
}

us_timestamp_t ticker_read_us(const ticker_data_t *const ticker)
{
    us_timestamp_t result;

    initialize(ticker);

    core_util_critical_section_enter();
    update_present_time(ticker);
    result = ticker->queue->present_time;
    core_util_critical_section_exit();

    return result;
}

int ticker_get_next_timestamp(const ticker_data_t *const ticker, timestamp_t *timestamp)
{
    int result = 0;

    // if head is NULL, there are no pending events
    core_util_critical_section_enter();
    if (ticker->queue->head != NULL) {
        *timestamp = ticker->queue->head->timestamp;
        result = 1;
    }
    core_util_critical_section_exit();

    return result;
}
