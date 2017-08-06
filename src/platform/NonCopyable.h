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
#ifndef MBINO_NON_COPYABLE_H
#define MBINO_NON_COPYABLE_H

namespace mbino {

    template<typename T>
    class NonCopyable {
    protected:
        NonCopyable() {}
        ~NonCopyable() {}

    private:
        NonCopyable(const NonCopyable&);
        NonCopyable& operator=(const NonCopyable&);
    };

}

#endif
