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
#ifndef MBINO_FILE_HANDLE_H
#define MBINO_FILE_HANDLE_H

#include "platform.h"
#include "NonCopyable.h"

namespace mbino {

    class FileHandle : private NonCopyable<FileHandle> {
    public:
        virtual ~FileHandle() {}

        virtual ssize_t read(void* buffer, size_t size) = 0;

        virtual ssize_t write(const void* buffer, size_t size) = 0;

        virtual off_t seek(off_t offset, int whence = SEEK_SET) = 0;

        virtual int close() = 0;

        // mbino restriction: not virtual
        off_t tell() {
            return seek(0, SEEK_CUR);
        }

        // mbino restriction: not virtual
        void rewind() {
            seek(0, SEEK_SET);
        }

        /* mbino restriction: these are  not supported (yet)
        virtual int sync() {
            return 0;
        }

        virtual int isatty() {
            return 0;
        }

        virtual off_t size();

        virtual int set_blocking(bool blocking) {
            return -1;
        }

        virtual short poll(short events) const {
            // Possible default for real files
            return POLLIN | POLLOUT;
        }

        bool writable() const {
            return poll(POLLOUT) & POLLOUT;
        }

        bool readable() const {
            return poll(POLLIN) & POLLIN;
        }

        virtual void sigio(Callback<void()> func) {
            // Default for real files. Do nothing for real files.
        }
        */

    };

    FILE* fdopen(FileHandle* fh, const char* mode);

}

#endif
