/* mbino - basic mbed APIs for the Arduino platform
 * Copyright (c) 2017 Thomas Kemmer
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

#include "platform/platform.h"

// include here so Arduino IDE sets correct include and library paths
#include <Arduino.h>

#ifdef DEVICE_I2C
// do *not* include Wire.h when using the Arduino IDE
#ifndef ARDUINO
#include <Wire.h>
#endif
#endif

#ifdef DEVICE_SPI
#include <SPI.h>
#endif

#include <math.h>
#include <time.h>

#include "drivers/AnalogIn.h"
#include "drivers/DigitalIn.h"
#include "drivers/DigitalInOut.h"
#include "drivers/DigitalOut.h"
#include "drivers/I2C.h"
#include "drivers/InterruptIn.h"
#include "drivers/PortIn.h"
#include "drivers/PortInOut.h"
#include "drivers/PortOut.h"
#include "drivers/PwmOut.h"
#include "drivers/RawSerial.h"
#include "drivers/SPI.h"
#include "drivers/Serial.h"
#include "drivers/Ticker.h"
#include "drivers/Timeout.h"
#include "drivers/Timer.h"
#include "drivers/TimerEvent.h"

//#include "platform/ATCmdParser.h"
#include "platform/Callback.h"
//#include "platform/CriticalSectionLock.h"
//#include "platform/DeepSleepLock.h"
//#include "platform/DirHandle.h"
#include "platform/FileHandle.h"
//#include "platform/FileSystemHandle.h"
//#include "platform/FunctionPointer.h"
//#include "platform/LocalFileSystem.h"
//#include "platform/mbed_application.h"
#include "platform/mbed_assert.h"
//#include "platform/mbed_debug.h"
#include "platform/mbed_error.h"
#include "platform/mbed_interface.h"
//#include "platform/mbed_poll.h"
#include "platform/mbed_rtc_time.h"
//#include "platform/mbed_sleep.h"
#include "platform/mbed_toolchain.h"
#include "platform/mbed_wait_api.h"

#include "platform/PlatformInit.h"

#endif
