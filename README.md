# mbino [![Build Status](https://travis-ci.org/tkem/mbino.svg?branch=master)](https://travis-ci.org/tkem/mbino/)

mbino is a library that provides some basic [mbed
OS](https://docs.mbed.com/docs/mbed-os-handbook/en/latest/) APIs for
the Arduino platform.

Currently, the following APIs are - at least partially - supported:

- [AnalogIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/AnalogIn/)
- [DigitalIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/DigitalIn/)
- [DigitalOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/DigitalOut/)
- [DigitalInOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/DigitalInOut/)
- [InterruptIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/InterruptIn/)
- [I2C](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/interfaces/digital/I2C/)
- [PortIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PortIn/)
- [PortOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PortOut/)
- [PortInOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PortInOut/)
- [PwmOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PwmOut/)
- [RawSerial](https://docs.mbed.com/docs/mbed-os-api/en/mbed-os-5.5/api/classmbed_1_1RawSerial.html)
- [Serial](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/interfaces/digital/Serial/)
- [SPI](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/interfaces/digital/SPI/)
- [Ticker](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/tasks/Ticker/)
- [Timeout](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/tasks/TimeOut/)
- [Timer](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/tasks/Timer/)
- [Wait](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/tasks/wait/)

mbino is *not* a full port of the mbed operating system to 8-bit
microcontrollers.  It just implements some of the basic C++ APIs that
are used when developing applications for mbed OS.  This means you
will still write "sketches" for your Arduino board, and you can even
use both mbed APIs and the "Arduino language", as well as native
Arduino libraries, in your programs.

So, why would you want to use this?

- You have to port existing mbed application code to the Arduino
  platform.

- You want to write cross-platform libraries that target both mbed and
  Arduino, using a single code base.

- You just happen to like mbed's C++ APIs, and don't want to go back.

Please note that this project is still at an early stage, and probably
not ready for production use yet.  Feel free to report any problems
using the project's [issue
tracker](https://github.com/tkem/mbino/issues/).


## Known Bugs and Limitations

- Only AVR 8-bit microcontroller boards are supported.  This means no
  support for Arduino Due or Zero, for example.

- To avoid ambiguities with Arduino's own global `Serial` and `SPI`
  objects, you should use the fully qualified class names
  `mbed::Serial` and `mbed::SPI` when using these APIs.

- Since the default AVR Libc `printf()` family of functions does not
  support floating point conversions, floating point values cannot be
  used with `Serial::printf()` or `RawSerial::printf()`.

- For portability (and so `millis()` and PWM outputs still work), the
  `Ticker` API uses the Timer0 comparison register for generating
  ticker interrupts.  Therefore, `Ticker` only provides a resolution
  of about one millisecond.

- The `PwmOut::period()` family of methods is only supported for PWM
  pins controlled by 16-bit timers, and is ignored for all other pins.
  Note that setting a period for one pin will also affect other pins
  controlled by the same timer.

- The `SPI` API is based on the Arduino [SPI
  library](https://www.arduino.cc/en/Reference/SPI).  Only 8-bit words
  are supported, and pin names passed to the constructor will be
  ignored (but should be set to `SPI_MOSI`, `SPI_MISO` and `SPI_SCK`
  for portability).

- The `I2C` API is based on the Arduino [Wire
  library](https://www.arduino.cc/en/Reference/Wire).  Since this
  library can substantially increase code size and RAM usage, I2C
  support is optional and disabled by default.  To enable I2C support,
  you have to `#include <Wire.h>` *before* `#include <mbed.h>`.  See
  the I2C [examples](examples/) for details.  Also note that the Wire
  library does not support some of the lower-level `I2C` API methods,
  and that you are limited to reading and writing 32 bytes at a time.
  As with the `SPI` API, pin names passed to the `I2C` constructor
  will be ignored for now, but should be set to `I2C_SDA` and
  `I2C_SCL` for portability.

- Although mbino has been designed so that you usually don't pay for
  what you don't use, there still may be some overhead involved when
  compared to "native" Arduino code.

- Keep in mind that you are still developing for an 8-bit platform, so
  only 8-bit variables can be read atomically, e.g. when passing data
  between an ISR and the main program.  Also note that `int` is only
  16 bits wide - that's why some APIs, such as the `wait_ms()` and
  `wait_us()` functions, have been changed to use `long` parameters.


## License

Copyright (c) 2017 Thomas Kemmer

mbed Microcontroller Library Copyright (c) 2006-2013 ARM Limited

Licensed under the [Apache License, Version
2.0](http://www.apache.org/licenses/LICENSE-2.0).
