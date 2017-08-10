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
#ifndef MBINO_CALLBACK_H
#define MBINO_CALLBACK_H

namespace mbino {

    template<typename F> class Callback;

    template<typename R>
    class Callback<R()> {

        template<typename Func, typename Arg>
        class function_context {
            Func _func;
            Arg* _arg;
        public:
            function_context(Func func, Arg* arg) : _func(func), _arg(arg) {}

            R operator()() const {
                return _func(_arg);
            }
        };

        template<typename Class, typename Method>
        class method_context {
            Class* _obj;
            Method _method;
        public:
            method_context(Class* obj, Method method) : _obj(obj), _method(method) {}

            R operator()() const {
                return (_obj->*_method)();
            }
        };

        union Func {
            Func() {}
            R (*fn)();
            function_context<R (*)(void*), void> fc;
            method_context<Func, R (Func::*)()> mc;
        } _func;

        R (*_thunk)(const void*);

    public:
        Callback(R (*func)() = 0) {
            if (!func) {
                _thunk = 0;
            } else {
                generate(func);
            }
        }

        template<typename T, typename U>
        Callback(R (*func)(T*), U* arg) {
            generate(function_context<R (*)(T*), T>(func, arg));
        }

        template<typename T, typename U>
        Callback(R (*func)(const T*), const U* arg) {
            generate(function_context<R (*)(const T*), const T>(func, arg));
        }

        template<typename T, typename U>
        Callback(R (*func)(volatile T*), volatile U* arg) {
            generate(function_context<R (*)(volatile T*), volatile T>(func, arg));
        }

        template<typename T, typename U>
        Callback(R (*func)(const volatile T*), const volatile U* arg) {
            generate(function_context<R (*)(const volatile T*), const volatile T>(func, arg));
        }

        template<typename T, typename U>
        Callback(U* obj, R (T::*method)()) {
            generate(method_context<T, R (T::*)()>(obj, method));
        }

        template<typename T, typename U>
        Callback(const U *obj, R (T::*method)() const) {
            generate(method_context<const T, R (T::*)() const>(obj, method));
        }

        template<typename T, typename U>
        Callback(volatile U* obj, R (T::*method)() volatile) {
            generate(method_context<volatile T, R (T::*)() volatile>(obj, method));
        }

        template<typename T, typename U>
        Callback(const volatile U* obj, R (T::*method)() const volatile) {
            generate(method_context<const volatile T, R (T::*)() const volatile>(obj, method));
        }

        R call() const {
            // TODO: evaluate simple switch/case on type flag
            return _thunk(&_func);
        }

        R operator()() const {
            return call();
        }

        operator bool() const {
            return _thunk != 0;
        }

    private:
        template<typename Func>
        void generate(const Func& f) {
            // FIXME: placement new not available?
            //new (&_func) Func(f);
            *reinterpret_cast<Func*>(&_func) = f;
            _thunk = &Callback::thunk<Func>;
        }

        template<typename Func>
        static R thunk(const void* p) {
            return (*static_cast<const Func*>(p))();
        }
    };

    template<typename R>
    Callback<R()> callback(R (*func)() = 0) {
        return Callback<R()>(func);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(R (*func)(T*), U* arg) {
        return Callback<R()>(func, arg);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(R (*func)(const T*), const U* arg) {
        return Callback<R()>(func, arg);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(R (*func)(volatile T*), volatile U* arg) {
        return Callback<R()>(func, arg);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(R (*func)(const volatile T*), const volatile U* arg) {
        return Callback<R()>(func, arg);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(U* obj, R (T::*method)()) {
        return Callback<R()>(obj, method);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(const U* obj, R (T::*method)() const) {
        return Callback<R()>(obj, method);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(volatile U* obj, R (T::*method)() volatile) {
        return Callback<R()>(obj, method);
    }

    template<typename T, typename U, typename R>
    Callback<R()> callback(const volatile U* obj, R (T::*method)() const volatile) {
        return Callback<R()>(obj, method);
    }

    template <typename R>
    Callback<R()> callback(const Callback<R()>& func) {
        return Callback<R()>(func);
    }
}

#endif
