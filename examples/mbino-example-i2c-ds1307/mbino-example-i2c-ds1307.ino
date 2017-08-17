#include "mbed.h"

// Get/set DS1307 RTC time and date

I2C i2c(I2C_SDA, I2C_SCL);

RawSerial pc(USBTX, USBRX);

const int addr = 0x68 << 1;  // 8bit I2C address

int bcddec8(uint8_t value)
{
    return 10 * (value >> 4) + (value & 0xf);
}

void setup() {
    // set clock to 17-08-17 23:59:59
    const char data[] = {
        0x00,  // word address
        0x55,  // seconds (BCD)
        0x59,  // minutes (BCD)
        0x23,  // hours (BCD)
        0x04,  // day of week (Monday=1)
        0x17,  // day of month (BCD)
        0x08,  // month (BCD)
        0x17,  // year (2-digit BCD)
        0x00   // control register
    };
    i2c.write(addr, data, sizeof data);
}

void loop() {
    char send[] = { 0 };  // word address
    i2c.write(addr, send, sizeof send, false);

    char recv[8];
    i2c.read(addr, recv, sizeof recv);
    pc.printf("%02d-%02d-%02d %02d:%02d:%02d [CTRL=0x%x]\r\n",
              bcddec8(recv[6]),
              bcddec8(recv[5]),
              bcddec8(recv[4]),
              bcddec8(recv[2]),
              bcddec8(recv[1]),
              bcddec8(recv[0] & 0x7f),
              recv[7]);

    wait(1);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
