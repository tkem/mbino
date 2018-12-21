/* mbino - mbed APIs for the Arduino platform
 * Copyright (c) 2017, 2018 Thomas Kemmer
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
#include "platform/mbed_interface.h"
#include "platform/mbed_retarget.h"
#include "platform/FileHandle.h"
#include "platform/Stream.h"

#include "hal/us_ticker_api.h"

#include <Arduino.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// TODO: POSIX APIs? new/delete? exit()?

#ifdef __WITH_AVRLIBC__

extern "C" int mbed_stdio_get(FILE* fp)
{
    mbed::FileHandle* fh = static_cast<mbed::FileHandle*>(fdev_get_udata(fp));
    unsigned char c;
    switch (fh->read(&c, 1)) {
    case 1:
        return c;
    case 0:
        return _FDEV_EOF;
    default:
        return _FDEV_ERR;
    }
}

extern "C" int mbed_stdio_put(char c, FILE *fp)
{
    mbed::FileHandle* fh = static_cast<mbed::FileHandle*>(fdev_get_udata(fp));
    return fh->write(&c, 1) == 1 ? 0 : -1;
}

FILE* mbed::fdopen(mbed::FileHandle *fh, const char *mode)
{
    bool rd = mode[0] == 'r' || mode[1] == '+';
    bool wr = mode[0] == 'w' || mode[1] == '+';
    FILE* fp = fdevopen(wr ? mbed_stdio_put : 0, rd ? mbed_stdio_get : 0);
    fdev_set_udata(fp, fh);
    return fp;
}

#else

static int mbed_read(void* obj, char* s, int n)
{
    return static_cast<mbed::FileHandle*>(obj)->read(s, n);
}

static int mbed_write(void* obj, const char* s, int n)
{
    return static_cast<mbed::FileHandle*>(obj)->write(s, n);
}

static fpos_t mbed_seek(void* obj, fpos_t offset, int whence)
{
    return static_cast<mbed::FileHandle*>(obj)->seek(offset, whence);
}

static int mbed_close(void* obj)
{
    return static_cast<mbed::FileHandle*>(obj)->close();
}

FILE* mbed::fdopen(FileHandle* fh, const char* mode)
{
    bool rd = mode[0] == 'r' || mode[1] == '+';
    bool wr = mode[0] == 'w' || mode[1] == '+';
    return funopen(fh, rd ? mbed_read : 0, wr ? mbed_write : 0, mbed_seek, mbed_close);
}

#endif

namespace mbed {
    void mbed_set_unbuffered_stream(FILE* fp)
    {
        setbuf(fp, 0);
    }

    int mbed_getc(FILE* fp)
    {
        return fgetc(fp);
    }

    char* mbed_gets(char* s, int size, FILE* fp)
    {
        return fgets(s, size, fp);
    }

    void remove_filehandle(mbed::FileHandle* /*fh*/)
    {
        // currently not needed(?)
    }

    // mbed_target_override_console() cannot be declared weak and
    // overridden in a single library, so this needs to be defined in
    // target

    MBED_WEAK FileHandle* mbed_override_console(int /*fd*/)
    {
        return NULL;
    }
}

#ifdef __WITH_AVRLIBC__
// not implemented in AVR libc
void setbuf(FILE *stream, char *buf)
{
}

int vsscanf(const char *s, const char *fmt, va_list ap)
{
    FILE f;
    f.flags = __SRD | __SSTR;
    f.buf = (char *)s;
    return vfscanf(&f, fmt, ap);
}
#endif

// TODO: exit() vs. _exit()
void exit(int status)
{
#if DEVICE_STDIO_MESSAGES
#if MBED_CONF_PLATFORM_STDIO_FLUSH_AT_EXIT
    fflush(stdout);
    fflush(stderr);
#endif
#endif
    if (status) {
        mbed_die();
    }

    while (1);
}

int atexit(void (*func)())
{
    // in mbed, this function never registers any handler...
    return 1;
}

#ifdef __WITH_AVRLIBC__
// AVR effectively limits CLOCKS_PER_SEC to 15 bits in <time.h> since
// pointers are treated as signed when converted to integers,
// e.g. (clock_t)(char *)62500U == 0xfffff424
char *_CLOCKS_PER_SEC_ = (char*)31250;
#endif

clock_t clock()
{
    // use micros() directly to avoid pulling in the whole us_ticker_api
    clock_t t = micros();
    t /= 1000000UL / CLOCKS_PER_SEC;
    return t;
}
