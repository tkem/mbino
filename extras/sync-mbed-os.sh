#!/bin/sh

cp $1/mbed.h src/
cp $1/drivers/*.* src/drivers/
cp $1/platform/*.* src/platform/
cp $1/hal/*.* src/hal/

# remove deprecated and unsupported files
rm src/drivers/InterruptManager.cpp
rm src/drivers/InterruptManager.h
rm src/drivers/mbed_lib.json
rm src/hal/mbed_critical_section_api.c
rm src/hal/mbed_itm_api.c
rm src/hal/mbed_ticker_api.c
rm src/platform/CallChain.cpp
rm src/platform/CallChain.h
rm src/platform/critical.h
rm src/platform/mbed_critical.c
rm src/platform/mbed_error.c
rm src/platform/mbed_lib.json
rm src/platform/mbed_retarget.cpp
rm src/platform/mbed_sdk_boot.c
rm src/platform/mbed_semihost_api.c
rm src/platform/mbed_sleep.h
rm src/platform/mbed_stats.c
rm src/platform/mbed_wait_api_no_rtos.c
rm src/platform/mbed_wait_api_rtos.cpp
rm src/platform/rtc_time.h
rm src/platform/semihost_api.h
rm src/platform/sleep.h
rm src/platform/toolchain.h
rm src/platform/wait_api.h
