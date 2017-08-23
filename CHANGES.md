## 0.5.0 (2017-08-23)

- Add `PwmOut` period and pulsewidth control.


## 0.4.0 (2017-08-21)

- Add `Serial` API.

- Add `PortIn` and `PortOut` examples.

- Temporarily remove `I2C` support (breaking change).


## 0.3.2 (2017-08-20)

- Add missing `va_end()` in `RawSerial::printf()`.

- Remove leftover header file.


## 0.3.1 (2017-08-18)

- Add device capability macros.


## 0.3.0 (2017-08-17)

- Add initial `I2C` API implementation.


## 0.2.2 (2017-08-16)

- Remove unnecessary member from `PortOut`.

- Add default baud rate to all `RawSerial` constructors.

- Eliminate unnecessary virtual destructors.

- Refactor inline methods.


## 0.2.1 (2017-08-15)

- Clean up timer/ticker implementation.


## 0.2.0 (2017-08-14)

- Assume a minimum of six digital pins for all platforms.

- Define `digitalPinToInterrupt()` for ATTiny.

- Check for known hardware pin names in `serial_init()`.


## 0.1.0 (2017-08-12)

- Initial release.
