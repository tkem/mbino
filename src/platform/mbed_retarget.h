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
#ifndef MBINO_MBED_RETARGET_H
#define MBINO_MBED_RETARGET_H

#include <stdlib.h>
#include <stdio.h>

#ifdef __AVR__
typedef int ssize_t;
typedef long off_t;
#endif

#ifdef getc
#undef getc
#endif

#ifdef putc
#undef putc
#endif

#ifdef __cplusplus
namespace mbino {

    class FileHandle;

    FILE* mbed_fdopen(FileHandle* fh, const char* mode);

    void mbed_set_unbuffered_stream(FILE* fp);

}
#endif

#endif
