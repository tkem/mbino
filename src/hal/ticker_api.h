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

#include <stdbool.h>
#include <stdint.h>

#define MBED_TICKER_INTERRUPT_TIMESTAMP_MAX_DELTA 0x70000000ULL

typedef uint32_t timestamp_t;

typedef uint64_t us_timestamp_t;

// mbino extension: change id type to intptr_t
typedef void (*ticker_event_handler)(intptr_t id);

typedef struct ticker_event_s {
    us_timestamp_t timestamp;
    intptr_t id;
    struct ticker_event_s *next;
    // mbino extension: store handler with ticker_event_t
    ticker_event_handler event_handler;
} ticker_event_t;

typedef struct {
    void (*init)(void);
    uint32_t (*read)(void);
    // TODO: disable_interrupt, clear_interrupt, fire_interrupt
    void (*set_interrupt)(timestamp_t timestamp);
} ticker_interface_t;

typedef struct {
    // mbino extension: moved event_handler to ticker_event_t
    ticker_event_t *head;
    us_timestamp_t present_time;
    bool initialized;
} ticker_event_queue_t;

typedef struct {
    const ticker_interface_t *interface;
    ticker_event_queue_t *queue;
} ticker_data_t;

#ifdef __cplusplus
extern "C" {
#endif

void ticker_irq_handler(const ticker_data_t *const ticker);

// mbino extension
void ticker_init_event(ticker_event_t *obj, ticker_event_handler handler, intptr_t id);

void ticker_remove_event(const ticker_data_t *const ticker, ticker_event_t *obj);

// mbino extension: moved id initialization to ticker_init_event()
void ticker_insert_event(const ticker_data_t *const ticker, ticker_event_t *obj, timestamp_t timestamp);

// mbino extension: moved id initialization to ticker_init_event()
void ticker_insert_event_us(const ticker_data_t *const ticker, ticker_event_t *obj, us_timestamp_t timestamp);

timestamp_t ticker_read(const ticker_data_t *const ticker);

us_timestamp_t ticker_read_us(const ticker_data_t *const ticker);

int ticker_get_next_timestamp(const ticker_data_t *const ticker, timestamp_t *timestamp);

#ifdef __cplusplus
}
#endif

#endif
