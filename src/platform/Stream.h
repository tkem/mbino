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
#ifndef MBINO_STREAM_H
#define MBINO_STREAM_H

#include "platform.h"
#include "FileHandle.h"
#include "NonCopyable.h"

#include <stdarg.h>

namespace mbino {

    // mbino restriction: FileLike not supported

    class Stream : public FileHandle, private NonCopyable<Stream> {
        FILE* _file;

    public:
        Stream();

        virtual ~Stream();

        int putc(int c) {
            return fputc(c, _file);
        }

        int puts(const char* s) {
            return fputs(s, _file);
        }

        int getc() {
            return fgetc(_file);
        }

        char* gets(char* s, int size) {
            return fgets(s, size, _file);
        }

        int printf(const char* format, ...);

        int scanf(const char* format, ...);

        int vprintf(const char* format, va_list args);

        int vscanf(const char* format, va_list args);

        operator FILE*() { return _file; }

    protected:
        /* TODO: not necessary for Serial
        virtual int close();
        virtual ssize_t write(const void* buffer, size_t length);
        virtual ssize_t read(void* buffer, size_t length);
        virtual off_t seek(off_t offset, int whence);
        virtual off_t tell();
        virtual void rewind();
        virtual int isatty();
        virtual int sync();
        virtual off_t size();
        */

        /* lock method stubs not supported
        virtual void lock() {}
        virtual void unlock() {}
        */

        virtual int _putc(int c) = 0;
        virtual int _getc() = 0;
    };

}

#endif
