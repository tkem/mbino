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
#ifndef MBINO_MBED_ASSERT_H
#define MBINO_MBED_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

void mbed_assert_internal(const char *expr, const char *file, int line);

#ifdef __cplusplus
}
#endif

#ifndef NDEBUG
#define MBED_ASSERT(expr)                                \
do {                                                     \
    if (!(expr)) {                                       \
        mbed_assert_internal(#expr, __FILE__, __LINE__); \
    }                                                    \
} while (0)
#else
#define MBED_ASSERT(expr) ((void)0)
#endif

#ifdef __cplusplus
#define MBED_STATIC_ASSERT(expr, msg) static_assert(expr, msg)
#else
#define MBED_STATIC_ASSERT(expr, msg) _Static_assert(expr, msg)
#endif

#define MBED_STRUCT_STATIC_ASSERT(expr, msg) int : (expr) ? 0 : -1

#endif
