/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
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
#ifdef ARDUINO_ARCH_AVR

#include "platform/FileHandle.h"

namespace mbino {

    static int put(char c, FILE* fp)
    {
        return static_cast<FileHandle*>(fdev_get_udata(fp))->_putc(c);
    }

    static int get(FILE* fp)
    {
        return static_cast<FileHandle*>(fdev_get_udata(fp))->_getc();
    }

    FILE* fdopen(FileHandle* fh, const char* mode)
    {
        // TODO: check mode
        FILE* fp = fdevopen(put, get);
        fdev_set_udata(fp, fh);
        return fp;
    }

}

#endif
