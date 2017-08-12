# mbino [![Build Status](https://travis-ci.org/tkem/mbino.svg?branch=master)](https://travis-ci.org/tkem/mbino/)

mbino is a library that provides some basic [mbed
OS](https://docs.mbed.com/docs/mbed-os-handbook/en/latest/) APIs for
the Arduino platform.

Currently, the following APIs are supported:

- [AnalogIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/AnalogIn/)
- [DigitalIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/DigitalIn/)
- [DigitalOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/DigitalOut/)
- [DigitalInOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/DigitalInOut/)
- [PortIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PortIn/)
- [PortOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PortOut/)
- [PortInOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PortInOut/)
- [InterruptIn](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/InterruptIn/)
- [PwmOut](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/io/PwmOut/)
- [RawSerial](https://docs.mbed.com/docs/mbed-os-api/en/mbed-os-5.5/api/classmbed_1_1RawSerial.html)
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

- You happen to like mbed's C++ APIs and just don't want to go back.

Please note that this project is still at an early stage, and probably
not ready for production use yet.  Feel free to report any problems
using the project's [issue
tracker](https://github.com/tkem/mbino/issues/).


## Known Bugs and Limitations

- Only AVR 8-bit microcontroller boards are supported.  This means no
  support for Arduino Due or Zero, for example.

- The mbed
  [Serial](https://docs.mbed.com/docs/mbed-os-api-reference/en/latest/APIs/interfaces/digital/Serial/)
  API is not provided, due to it's use of streams and to prevent name
  clashes with Arduino's own `Serial` object.
  [RawSerial](https://docs.mbed.com/docs/mbed-os-api/en/mbed-os-5.5/api/classmbed_1_1RawSerial.html)
  is supported, though not quite complete.

- Since the default AVR Libc `printf()` family of functions do not
  support floating point conversions, floating point values cannot be
  used with `RawSerial::printf()`.

- Although mbino has been designed so that you usually don't pay for
  what you don't use, there may be some overhead involved when
  compared to "native" Arduino code.

- Keep in mind that you are still developing for an 8-bit platform, so
  only 8-bit variables can be read atomically, e.g. when passing data
  between an ISR and the main program.


## License

Copyright (c) 2017 Thomas Kemmer

mbed Microcontroller Library Copyright (c) 2006-2013 ARM Limited

Licensed under the [Apache License, Version
2.0](http://www.apache.org/licenses/LICENSE-2.0).
