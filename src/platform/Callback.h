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

    template<typename R, typename... Args>
    class Callback<R(Args...)> {

        template<typename Func, typename Arg>
        class function_context {
            Func _func;
            Arg* _arg;

        public:
            function_context(Func func, Arg* arg) : _func(func), _arg(arg) {}

            R operator()(Args... args) const {
                return _func(_arg, args...);
            }
        };

        template<class Class, typename Method>
        class method_context {
            Class* _obj;
            Method _method;

        public:
            method_context(Class* obj, Method method) : _obj(obj), _method(method) {}

            R operator()(Args... args) const {
                return (_obj->*_method)(args...);
            }
        };

        union Func {
            Func() {}
            R (*fn)(Args...);
            function_context<R (*)(void*, Args...), void> fc;
            method_context<Func, R (Func::*)(Args...)> mc;
        } _func;

        R (*_thunk)(const void*, Args...);

    public:
        Callback() : _thunk(0) {}

        Callback(R (*func)(Args...)) {
            if (!func) {
                _thunk = 0;
            } else {
                init(func);
            }
        }

        template<typename T, typename U>
        Callback(R (*func)(T*, Args...), U* arg) {
            init(function_context<R (*)(T*, Args...), T>(func, arg));
        }

        template<typename T, typename U>
        Callback(R (*func)(const T*, Args...), const U* arg) {
            init(function_context<R (*)(const T*, Args...), const T>(func, arg));
        }

        template<typename T, typename U>
        Callback(R (*func)(volatile T*, Args...), volatile U* arg) {
            init(function_context<R (*)(volatile T*, Args...), volatile T>(func, arg));
        }

        template<typename T, typename U>
        Callback(R (*func)(const volatile T*, Args...), const volatile U* arg) {
            init(function_context<R (*)(const volatile T*, Args...), const volatile T>(func, arg));
        }

        template<typename T, typename U>
        Callback(U* obj, R (T::*method)(Args...)) {
            init(method_context<T, R (T::*)(Args...)>(obj, method));
        }

        template<typename T, typename U>
        Callback(const U *obj, R (T::*method)(Args...) const) {
            init(method_context<const T, R (T::*)(Args...) const>(obj, method));
        }

        template<typename T, typename U>
        Callback(volatile U* obj, R (T::*method)(Args...) volatile) {
            init(method_context<volatile T, R (T::*)(Args...) volatile>(obj, method));
        }

        template<typename T, typename U>
        Callback(const volatile U* obj, R (T::*method)(Args...) const volatile) {
            init(method_context<const volatile T, R (T::*)(Args...) const volatile>(obj, method));
        }

        R call(Args... args) const {
            return _thunk(&_func, args...);
        }

        R operator()(Args... args) const {
            return call(args...);
        }

        operator bool() const {
            return _thunk != 0;
        }

    private:
        template<typename Context>
        void init(const Context& context) {
            *reinterpret_cast<Context*>(&_func) = context;
            _thunk = &Callback::thunk<Context>;
        }

        template<typename Context>
        static R thunk(const void* p, Args... args) {
            return (*static_cast<const Context*>(p))(args...);
        }
    };

    template<typename R, typename... Args>
    Callback<R(Args...)> callback(R (*func)(Args... args) = 0) {
        return Callback<R(Args...)>(func);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(R (*func)(T*, Args...), U* arg) {
        return Callback<R(Args...)>(func, arg);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(R (*func)(const T*, Args...), const U* arg) {
        return Callback<R(Args...)>(func, arg);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(R (*func)(volatile T*, Args...), volatile U* arg) {
        return Callback<R(Args...)>(func, arg);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(R (*func)(const volatile T*, Args...), const volatile U* arg) {
        return Callback<R(Args...)>(func, arg);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(U* obj, R (T::*method)(Args...)) {
        return Callback<R(Args...)>(obj, method);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(const U* obj, R (T::*method)(Args...) const) {
        return Callback<R(Args...)>(obj, method);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(volatile U* obj, R (T::*method)(Args...) volatile) {
        return Callback<R(Args...)>(obj, method);
    }

    template<typename T, typename U, typename R, typename... Args>
    Callback<R(Args...)> callback(const volatile U* obj, R (T::*method)(Args...) const volatile) {
        return Callback<R(Args...)>(obj, method);
    }

    template <typename R, typename... Args>
    Callback<R(Args...)> callback(const Callback<R(Args...)>& func) {
        return Callback<R(Args...)>(func);
    }
}

#endif
