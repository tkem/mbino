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
#ifndef MBINO_MBED_TOOLCHAIN_H
#define MBINO_MBED_TOOLCHAIN_H

#ifndef __GNUC__
#error "This compiler is not supported."
#endif

#ifndef MBED_PACKED
#define MBED_PACKED(struct) struct __attribute__((packed))
#endif

#ifndef MBED_ALIGN
#define MBED_ALIGN(N) __attribute__((aligned(N)))
#endif

#ifndef MBED_UNUSED
#define MBED_UNUSED __attribute__((__unused__))
#endif

#ifndef MBED_WEAK
#define MBED_WEAK __attribute__((weak))
#endif

#ifndef MBED_PURE
#define MBED_PURE __attribute__((const))
#endif

#ifndef MBED_NOINLINE
#define MBED_NOINLINE __attribute__((noinline))
#endif

#ifndef MBED_FORCEINLINE
#define MBED_FORCEINLINE static inline __attribute__((always_inline))
#endif

#ifndef MBED_NORETURN
#define MBED_NORETURN __attribute__((noreturn))
#endif

#ifndef MBED_UNREACHABLE
#define MBED_UNREACHABLE __builtin_unreachable()
#endif

#ifndef MBED_DEPRECATED
#define MBED_DEPRECATED(M) __attribute__((deprecated(M)))
#endif

#define MBED_DEPRECATED_SINCE(D, M) MBED_DEPRECATED(M " [since " D "]")

#ifndef MBED_CALLER_ADDR
#define MBED_CALLER_ADDR() __builtin_extract_return_addr(__builtin_return_address(0))
#endif

#ifndef MBED_SECTION
#define MBED_SECTION(name) __attribute__ ((section (name)))
#endif

#ifndef MBED_PRINTF
#define MBED_PRINTF(format_idx, first_param_idx) __attribute__ ((__format__(__printf__, format_idx, first_param_idx)))
#endif

#ifndef MBED_PRINTF_METHOD
#define MBED_PRINTF_METHOD(format_idx, first_param_idx) __attribute__ ((__format__(__printf__, format_idx+1, first_param_idx+1)))
#endif

#ifndef MBED_SCANF
#define MBED_SCANF(format_idx, first_param_idx) __attribute__ ((__format__(__scanf__, format_idx, first_param_idx)))
#endif

#ifndef MBED_SCANF_METHOD
#define MBED_SCANF_METHOD(format_idx, first_param_idx) __attribute__ ((__format__(__scanf__, format_idx+1, first_param_idx+1)))
#endif

#endif
