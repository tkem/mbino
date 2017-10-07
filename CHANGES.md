1.0.1 (2017-10-07)
------------------

- Improve `mbed_die()` code size.


1.0.0 (2017-10-06)
------------------

- Code cleanup and refactoring.


0.10.1 (2017-10-06)
-------------------

- Improve error handling for ATmega32U based boards.

- Use `DEVICE_STDIO_MESSAGES` as intended.

- Refactor retargetting and `stdio` handling.


0.10.0 (2017-10-04)
-------------------

- Improve `stdio` and runtime error messages support.

- Add `Time` and `RTC` APIs.

- Add mbed assert macros.


0.9.3 (2017-10-03)
------------------

- Add basic runtime error handling.


0.9.2 (2017-09-27)
------------------

- Refactor target headers.

- Clean up serial API implementation.

- Add internal `serial_stream` API.


0.9.1 (2017-09-25)
------------------

- Extend `FileHandle` implementation.

- Use `analogReadResolution()` if available.

- Remove custom `serial_puts()`.


0.9.0 (2017-09-22)
------------------

- Add serial event handling.

- Add basic `FileHandle` API.

- Refactor `gpio_irq_api`.


0.8.3 (2017-09-20)
------------------

- Refactor `us_ticker_read()`.

- Refactor `gpio_irq_api` implementation.

- Implement missing core util functions.

- General interface cleanup.


0.8.2 (2017-09-19)
------------------

- Move `PortNames.h` to architecture directory.

- Reformat static inline functions.

- Add `gpio_object.h`.


0.8.1 (2017-09-19)
------------------

- Refactor HAL layer:

  * make HAL interfaces `extern "C"`.
  * add `targets` folder.
  * prepare for multiple architectures.


0.8.0 (2017-09-13)
------------------

- Add multi-parameter callbacks.

- Use `pgm_read_ptr()` for accessing timer registers.


0.7.1 (2017-09-11)
------------------

- Re-enable `PwmOut`.

- Fix compiler warnings in several examples.

- Code cleanup.


0.7.0 (2017-09-09)
------------------

- Remove `SoftwareSerial` support (breaking change).

- Remove `PWM_OUT`, `LED2`, `LED3`, `LED4` and `BUTTON1` pin name
  constants (breaking change).

- Fix I2C LM75 temperature sensor example.


0.6.4 (2017-09-07)
------------------

- Fix template parameter deduction error in `SerialBase` constructor.

- Fix timer control register table.


0.6.3 (2017-09-06)
------------------

- Remove `PwmOut` destructor.

- Clean up `AnalogIn` API.

- Update license headers.


0.6.2 (2017-09-05)
------------------

- Refactor (and fix) `PwmOut` API.


0.6.1 (2017-09-01)
------------------

- Fix `SPI::write()`.

- Add 16-bit SPI support (experimental).

- Add SPI EEPROM 25LC040 example.


0.6.0 (2017-08-24)
------------------

- Add optional `I2C` API.

- Add `SPI` API.


0.5.0 (2017-08-23)
------------------

- Add `PwmOut` period and pulsewidth control.


0.4.0 (2017-08-21)
------------------

- Add `Serial` API.

- Add `PortIn` and `PortOut` examples.

- Temporarily remove `I2C` support (breaking change).


0.3.2 (2017-08-20)
------------------

- Add missing `va_end()` in `RawSerial::printf()`.

- Remove leftover header file.


0.3.1 (2017-08-18)
------------------

- Add device capability macros.


0.3.0 (2017-08-17)
------------------

- Add initial `I2C` API implementation.


0.2.2 (2017-08-16)
------------------

- Remove unnecessary member from `PortOut`.

- Add default baud rate to all `RawSerial` constructors.

- Eliminate unnecessary virtual destructors.

- Refactor inline methods.


0.2.1 (2017-08-15)
------------------

- Clean up timer/ticker implementation.


0.2.0 (2017-08-14)
------------------

- Assume a minimum of six digital pins for all platforms.

- Define `digitalPinToInterrupt()` for ATTiny.

- Check for known hardware pin names in `serial_init()`.


0.1.0 (2017-08-12)
------------------

- Initial release.
