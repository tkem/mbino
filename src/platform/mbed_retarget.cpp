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
#include "platform/mbed_retarget.h"
#include "platform/FileHandle.h"

#include <stdio.h>

namespace mbino {

#ifdef __AVR__
    static int put(char c, FILE* fp)
    {
        FileHandle* fh = static_cast<FileHandle*>(fdev_get_udata(fp));
        return fh->write(&c, 1) == 1 ? 0 : -1;
    }

    static int get(FILE* fp)
    {
        unsigned char c;
        FileHandle* fh = static_cast<FileHandle*>(fdev_get_udata(fp));
        switch (fh->read(&c, 1)) {
        case 1:
            return c;
        case 0:
            return _FDEV_EOF;
        default:
            return _FDEV_ERR;
        }
    }

    FILE* mbed_fdopen(FileHandle* fh, const char* mode)
    {
        bool rd = mode[0] == 'r' || mode[1] == '+';
        bool wr = mode[0] == 'w' || mode[1] == '+';
        FILE* fp = fdevopen(wr ? put : 0, rd ? get : 0);
        fdev_set_udata(fp, fh);
        return fp;
    }

    void mbed_set_unbuffered_stream(FILE*)
    {
        // unbuffered by default
    }
#else
    static int read(void* obj, char* s, int n)
    {
        return static_cast<FileHandle*>(obj)->read(s, n);
    }

    static int write(void* obj, const char* s, int n)
    {
        return static_cast<FileHandle*>(obj)->write(s, n);
    }

    static fpos_t seek(void* obj, fpos_t offset, int whence)
    {
        return static_cast<FileHandle*>(obj)->seek(offset, whence);
    }

    static int close(void* obj)
    {
        return static_cast<FileHandle*>(obj)->close();
    }

    FILE* mbed_fdopen(FileHandle* fh, const char* mode)
    {
        bool rd = mode[0] == 'r' || mode[1] == '+';
        bool wr = mode[0] == 'w' || mode[1] == '+';
        return funopen(fh, rd ? read : 0, wr ? write : 0, seek, close);
    }

    void mbed_set_unbuffered_stream(FILE* fp)
    {
        setbuf(fp, 0);
    }
#endif
}
