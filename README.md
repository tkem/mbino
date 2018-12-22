# mbino [![Build Status](https://travis-ci.org/tkem/mbino.svg?branch=master)](https://travis-ci.org/tkem/mbino/)

mbino is a library that provides several [mbed OS
2](https://os.mbed.com/handbook/Homepage) APIs for the Arduino
platform.

Currently, the following APIs are - at least partially - supported:


| [__AnalogIn__](https://os.mbed.com/docs/latest/apis/analogin.html) | [__Assert__](https://os.mbed.com/docs/latest/apis/assert.html) | [__BusIn__](https://os.mbed.com/docs/latest/apis/busin.html) | [__BusOut__](https://os.mbed.com/docs/latest/apis/busout.html)
| :--- | :--- | :--- | :---
| [__BusInOut__ ](https://os.mbed.com/docs/latest/apis/businout.html) | [__Callback__](https://os.mbed.com/docs/latest/apis/callback.html) | [__CriticalSectionLock__](https://os.mbed.com/docs/latest/apis/criticalsectionlock.html) | [__Debug__](https://os.mbed.com/docs/latest/apis/debug.html)
| [__DigitalIn__](https://os.mbed.com/docs/latest/apis/digitalin.html) | [__DigitalOut__](https://os.mbed.com/docs/latest/apis/digitalout.html) | [__DigitalInOut__](https://os.mbed.com/docs/latest/apis/digitalinout.html) | [__Error handling__](https://os.mbed.com/docs/latest/apis/error-handling.html)
| [__InterruptIn__](https://os.mbed.com/docs/latest/apis/interruptin.html) | [__I2C__](https://os.mbed.com/docs/latest/apis/i2c.html) | [__I2CSlave__](https://os.mbed.com/docs/latest/apis/i2cslave.html) | [__NonCopyable__](https://os.mbed.com/docs/latest/apis/noncopyable.html)
| [__PortIn__](https://os.mbed.com/docs/latest/apis/portin.html) | [__PortOut__](https://os.mbed.com/docs/latest/apis/portout.html) | [__PortInOut__](https://os.mbed.com/docs/latest/apis/portinout.html) | [__PwmOut__](https://os.mbed.com/docs/latest/apis/pwmout.html)
| [__RawSerial__](https://os.mbed.com/docs/latest/apis/rawserial.html) | [__Serial__](https://os.mbed.com/docs/latest/apis/serial.html) | [__SPI__](https://os.mbed.com/docs/latest/apis/spi.html) | [__SPISlave__](https://os.mbed.com/docs/latest/apis/spislave.html)
| [__Ticker__](https://os.mbed.com/docs/latest/apis/ticker.html) | [__Timeout__](https://os.mbed.com/docs/latest/apis/timeout.html) | [__Timer__](https://os.mbed.com/docs/latest/apis/timer.html) | [__Wait__](https://os.mbed.com/docs/latest/apis/wait.html)

For an overview on how to use these, have a look at (and try out!) the
[example programs](examples).


## Known Bugs and Limitations

- Only AVR 8-bit microcontroller boards are supported.  This means no
  support for Arduino Due, Zero or M0, etc.

- Although mbed programs usually have a `main()` function, you can
  still write
  [sketches](examples/ArduinoSketch_HelloWorld/ArduinoSketch_HelloWorld.ino)
  based on `setup()` and `loop()`, if you want to.  This can be useful
  to sprinkle an existing sketch with `printf()` statements for
  debugging, for example.

- The standard file descriptors `stdin`, `stdout`, and `stderr` are
  set up by default to point to Arduino's serial monitor, so that
  `printf()` et al. work out of the box.  However, with the current
  implementation, this leads to a significant increase in code size,
  even if none of the `stdio` functions are ever used.  If you are
  running out of space, define `MBINO_CONF_PLATFORM_STDIO` to `0` in
  [`mbed_config.h`](src/mbed_config.h) to disable `stdio` support.

- Since the default AVR Libc `printf()` family of functions does not
  support floating point conversions, floating point values cannot be
  used with `printf()`, `Serial::printf()` or `RawSerial::printf()`.

- For portability (and so `millis()` and PWM still work), the Ticker
  API uses the `Timer0` comparison register for generating ticker
  interrupts.  Therefore, `Ticker` only provides a resolution of about
  one millisecond.

- The `PwmOut::period()` and `PwmOut::pulsewidth()` methods are only
  supported for PWM pins controlled by 16-bit timers, e.g. pins 9 and
  10 on the Arduino Uno.  For now, these methods are ignored for all
  other pins.  Note that setting a period for one pin will also affect
  other pins controlled by the same timer.

- To avoid ambiguities with Arduino's own global `Serial` object, you
  have to use the fully qualified class name `mbed::Serial` when using
  the mbed Serial API.

- Keep in mind that you are still developing for an 8-bit platform, so
  only 8-bit variables can be read atomically, e.g. when passing data
  between an ISR and your main program.  Also note that `int` on AVR
  is only 16 bits wide - that's why some APIs, such as those taking
  `baud` or `hz` parameters have been changed from `int` to `long`.


## License

Copyright (c) 2017, 2018 Thomas Kemmer

mbed Microcontroller Library Copyright (c) 2006-2018 ARM Limited

Licensed under the [Apache License, Version
2.0](http://www.apache.org/licenses/LICENSE-2.0).
