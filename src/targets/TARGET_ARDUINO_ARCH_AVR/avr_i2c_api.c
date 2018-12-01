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
#ifdef ARDUINO_ARCH_AVR

#include "hal/i2c_api.h"

#if DEVICE_I2C

#include <Arduino.h>
#include <util/twi.h>

#include "platform/mbino_error.h"

#define I2C_INVARG MBED_MAKE_ERROR(MBED_MODULE_DRIVER_I2C, MBED_ERROR_CODE_INVALID_ARGUMENT)

static void i2c_init_pin(PinName pin)
{
    uint8_t port = digitalPinToPort(pin);
    uint8_t mask = digitalPinToBitMask(pin);
    volatile uint8_t *reg = portOutputRegister(port);
    uint8_t sreg = SREG;
    cli();
    *reg |= mask;
    SREG = sreg;
}

static int i2c_byte_write_ex(i2c_t *obj, int data, int last)
{
    // TWEA is only used in slave mode
    TWDR = data;
    if (last) {
        TWCR = (1 << TWINT) | (1 << TWEN);
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    }
    while (!(TWCR & (1 << TWINT)));
    switch (TW_STATUS) {
    case TW_MR_SLA_ACK:
    case TW_MT_DATA_ACK:
    case TW_MT_SLA_ACK:
    case TW_ST_DATA_ACK:
        return 1;
    case TW_ST_LAST_DATA:  // actually an error - set TWSTO to reset TWI?
        return 1;
    case TW_MR_SLA_NACK:
    case TW_MT_DATA_NACK:
    case TW_MT_SLA_NACK:
    case TW_ST_DATA_NACK:
        return 0;
    default:
        return 2;  // timeout(?)
    }
}

void i2c_init(i2c_t *obj, PinName sda, PinName scl)
{
    if (sda == I2C_SDA && scl == I2C_SCL) {
        obj->slave = false;
        i2c_init_pin(I2C_SDA);
        i2c_init_pin(I2C_SCL);
        TWSR = 0;
        TWBR = (F_CPU / 100000L - 16) / 2;
        TWCR = (1 << TWEN);
    } else {
        MBINO_ERROR(I2C_INVARG, "I2C pin mapping failed");
    }
}

void i2c_frequency(i2c_t *obj, long hz)
{
    TWBR = (F_CPU / hz - 16) / 2;
}

int i2c_start(i2c_t *obj)
{
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    switch (TW_STATUS) {
    case TW_START:
    case TW_REP_START:
        return 0;
    default:
        return 1;
    }
}

int i2c_stop(i2c_t *obj)
{
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
    return 0;
}

int i2c_read(i2c_t *obj, int address, char *data, int length, int stop)
{
    i2c_start(obj);
    if (i2c_byte_write(obj, address | TW_READ) != 1) {
        return I2C_ERROR_NO_SLAVE;
    }
    for (int n = 0; n != length; ++n) {
        int r = i2c_byte_read(obj, n == length - 1);
        if (r >= 0) {
            data[n] = r;
        } else {
            length = n;
            break;
        }
    }
    if (stop) {
        i2c_stop(obj);
    }
    return length;
}

int i2c_write(i2c_t *obj, int address, const char *data, int length, int stop)
{
    i2c_start(obj);
    if (i2c_byte_write(obj, address | TW_WRITE) != 1) {
        return I2C_ERROR_NO_SLAVE;
    }
    for (int n = 0; n != length; ++n) {
        if (i2c_byte_write(obj, data[n]) != 1) {
            length = n + 1;
            break;
        }
    }
    if (stop) {
        i2c_stop(obj);
    }
    return length;
}

void i2c_reset(i2c_t *obj)
{
    // send stop, as suggested in i2c_api.h
    i2c_stop(obj);
    i2c_init(obj, I2C_SDA, I2C_SCL);
}

int i2c_byte_read(i2c_t *obj, int last)
{
    if (last) {
        TWCR = (1 << TWINT) | (1 << TWEN);
    } else {
        TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    }
    while (!(TWCR & (1 << TWINT)));
    switch (TW_STATUS) {
    case TW_MR_DATA_ACK:
    case TW_MR_DATA_NACK:
    case TW_SR_DATA_ACK:
    case TW_SR_DATA_NACK:
    case TW_SR_GCALL_DATA_ACK:
    case TW_SR_GCALL_DATA_NACK:
        return TWDR;
    case TW_SR_STOP:
        return -1;
    default:
        return -1;
    }
}

int i2c_byte_write(i2c_t *obj, int data)
{
    return i2c_byte_write_ex(obj, data, 0);
}

void i2c_slave_mode(i2c_t *obj, int enable_slave)
{
    // currently not used
    obj->slave = (enable_slave != 0);
}

int i2c_slave_receive(i2c_t *obj)
{
    TWCR = (1 << TWINT) | (1 << TWEA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    switch (TW_STATUS) {
    case TW_ST_SLA_ACK:
    case TW_ST_ARB_LOST_SLA_ACK:
        return 1;
    case TW_SR_GCALL_ACK:
    case TW_SR_ARB_LOST_GCALL_ACK:
        return 2;
    case TW_SR_SLA_ACK:
    case TW_SR_ARB_LOST_SLA_ACK:
        return 3;
    default:
        return 0;
    }
}

int i2c_slave_read(i2c_t *obj, char *data, int length)
{
    for (int n = 0; n != length; ++n) {
        int r = i2c_byte_read(obj, n == length - 1);
        if (r >= 0) {
            data[n] = r;
        } else {
            length = n;
            break;
        }
    }
    return length;
}

int i2c_slave_write(i2c_t *obj, const char *data, int length)
{
    for (int n = 0; n != length; ++n) {
        if (i2c_byte_write_ex(obj, data[n], n == length - 1) != 1) {
            length = n + 1;
            break;
        }
    }
    return length;
}

void i2c_slave_address(i2c_t *obj, int idx, uint32_t address, uint32_t mask)
{
    TWAR = address;
}

#endif

#endif
