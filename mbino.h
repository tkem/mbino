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
#ifndef MBINO_H
#define MBINO_H

#include <Arduino.h>

namespace mbino {
    typedef uint8_t PinName;

    enum PinDirection {
        PIN_INPUT = INPUT,
        PIN_OUTPUT = OUTPUT
    };

    enum PinMode {
        PullNone = INPUT,
        PullUp = INPUT_PULLUP,
#ifdef INPUT_PULLDOWN
        PullDown = INPUT_PULLDOWN,
#endif
        PullDefault = PullNone
    };

    template<typename T>
    class NonCopyable {
    protected:
        NonCopyable() {}
        ~NonCopyable() {}

    private:
        NonCopyable(const NonCopyable&);
        NonCopyable& operator=(const NonCopyable&);
    };

    template<typename F> class Callback;

    template<typename R>
    class Callback<R()> {
        class cls;  // placeholder

    public:
        Callback() : thunk(&Callback::nop_thunk) {}

        Callback(R (*func)()) : thunk(&Callback::func_thunk) {
            cb.func = func;
        }

        template<typename T>
        Callback(T* obj, R (T::*method)()) : thunk(&Callback::method_thunk<T>) {
            cb.method.obj = obj;
            cb.method.method = reinterpret_cast<R (cls::*)()>(method);
        }

        R operator()() {
            return (this->*thunk)();
        }

    private:
        union {
            R (*func)();
            struct {
                void* obj;
                R (cls::*method)();
            } method;
        } cb;

        R (Callback::*thunk)();

    private:
        R nop_thunk() {
        }

        R func_thunk() {
            return (*cb.func)();
        }

        template<typename T>
        R method_thunk() {
            T* obj = static_cast<T*>(cb.method.obj);
            R (T::*method)() = reinterpret_cast<R (T::*)()>(cb.method.method);
            return (obj->*method)();
        }
    };

    template<typename R>
    inline Callback<R()> callback(R (*func)()) {
        return Callback<R()>(func);
    }

    template<typename R, typename T>
    inline Callback<R()> callback(T* obj, R (T::*method)()) {
        return Callback<R()>(obj, method);
    }

    class AnalogIn {
        PinName _pin;

    public:
        AnalogIn(PinName pin) : _pin(pin) {
            ::pinMode(pin, INPUT);
        }

        float read() {
            return float(::analogRead(_pin)) * (1.0f / float(0x3FF));
        }

        uint16_t read_u16() {
            uint16_t value = ::analogRead(_pin);
            // 10-bit to 16-bit conversion
            return (value << 6) | (value >> 4);
        }

        operator float() {
            return read();
        }
    };

    class AnalogOut {
        PinName _pin;

    public:
        AnalogOut(PinName pin) : _pin(pin) {
            ::pinMode(pin, OUTPUT);
        }

        void write(float value) {
            if (value < 0.0f) {
                ::analogWrite(_pin, 0);
            } else if (value > 1.0f) {
                ::analogWrite(_pin, 255);
            } else {
                ::analogWrite(_pin, uint8_t(value * 255.0f));
            }
        }

        void write_u16(uint16_t value) {
            ::analogWrite(_pin, value >> 8);
        }

        float read() {
            return float(::analogRead(_pin)) * (1.0f / float(0x3FF));
        }

        AnalogOut& operator=(float value) {
            write(value);
            return *this;
        }

        AnalogOut& operator=(AnalogOut& rhs) {
            write(rhs.read());
            return *this;
        }
    };

    class DigitalIn {
        PinName _pin;

    public:
        DigitalIn(PinName pin) : _pin(pin) {
            mode(PullDefault);
        }

        DigitalIn(PinName pin, PinMode pull) : _pin(pin) {
            mode(pull);
        }

        int read() {
            return ::digitalRead(_pin);
        }

        void mode(PinMode pull) {
            ::pinMode(_pin, pull);
        }

        int is_connected() {
            return true;  // TODO: NC?
        }

        operator int() {
            return read();
        }
    };

    class DigitalOut {
        PinName _pin;

    public:
        DigitalOut(PinName pin) : _pin(pin) {
            ::pinMode(pin, OUTPUT);
        }

        DigitalOut(PinName pin, uint8_t value) : _pin(pin) {
            ::pinMode(pin, OUTPUT);
            write(value);
        }

        void write(uint8_t value) {
            ::digitalWrite(_pin, value);
        }

        int read() {
            return ::digitalRead(_pin);
        }

        int is_connected() {
            return true;  // TODO: NC?
        }

        DigitalOut& operator=(uint8_t value) {
            write(value);
            return *this;
        }

        DigitalOut& operator=(DigitalOut& rhs) {
            write(rhs.read());
            return *this;
        }

        operator int() {
            return read();
        }
    };

    class DigitalInOut {
        PinName _pin;

    public:
        DigitalInOut(PinName pin) : _pin(pin) {
            ::pinMode(_pin, INPUT);
        }

        DigitalInOut(PinName pin, PinDirection direction, PinMode pull, uint8_t value) : _pin(pin) {
            ::pinMode(pin, direction);
            // FIXME: honor direction
            mode(pull);
            write(value);
        }

        void write(uint8_t value) {
            ::digitalWrite(_pin, value);
        }

        int read() {
            return ::digitalRead(_pin);
        }

        void output() {
            ::pinMode(_pin, OUTPUT);
        }

        void input() {
            // FIXME: remember pull mode
            ::pinMode(_pin, INPUT);
        }

        void mode(PinMode pull) {
            // FIXME: remember pull mode
            ::pinMode(_pin, pull);
        }

        int is_connected() {
            return true;  // TODO: NC?
        }

        DigitalInOut& operator=(uint8_t value) {
            write(value);
            return *this;
        }

        DigitalInOut& operator=(DigitalInOut& rhs) {
            write(rhs.read());
            return *this;
        }

        operator int() {
            return read();
        }
    };

    class InterruptIn : private NonCopyable<InterruptIn> {
        PinName _pin;
        Callback<void()> _rise;
        Callback<void()> _fall;

    public:
        InterruptIn(PinName pin);
        ~InterruptIn();

        int read() {
            return ::digitalRead(_pin);
        }

        void rise(Callback<void()> func) {
            _rise = func;
        }

        void fall(Callback<void()> func) {
            _fall = func;
        }

        void mode(PinMode pull) {
            ::pinMode(_pin, pull);
        }

        // TODO: enable_irq()?
        // TODO: disable_irq()?

        operator int() {
            return read();
        }

        void isr(); // TODO: statis isr(id)?
    };

    class Timer : private NonCopyable<Timer> {
        unsigned long _micros;
        // TODO: running, extended us interval?

    public:
        void start() {
            _micros = micros();
        }

        void stop() {
            // TODO: running
        }

        void reset() {
            _micros = micros();
        }

        float read() {
            return read_us() / 1000000.f;
        }

        unsigned long read_ms() {
            return read_us() / 1000;
        }

        unsigned long read_us() {
            return micros() - _micros;
        }

        operator float() {
            return read();
        }

        unsigned long read_high_resolution_us() {
            // TODO: improved resolution?
            return read_us();
        }
    };

    inline void wait_ms(unsigned long ms) {
        ::delay(ms);
    }

    inline void wait_us(unsigned int us) {
        // FIXME: delayMicroseconds unly allows 16bit values?
        ::delayMicroseconds(us);
    }

    inline void wait(float s) {
        // FIXME: use wait_us() adapted for larger values
        wait_ms(s * 1000U);
    }
}

#endif
