// include the Wire library to enable I2C support
#ifdef ARDUINO
#include <Wire.h>
#endif

#include "mbed.h"

// Read temperature from LM75BD

I2C i2c(I2C_SDA, I2C_SCL);

RawSerial pc(USBTX, USBRX);

const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

void setup() {
}

void loop() {
    char cmd[2];
    cmd[0] = 0x01;
    cmd[1] = 0x00;

    i2c.write(addr8bit, cmd, 2);

    wait(0.5);

    cmd[0] = 0x00;
    i2c.write(addr8bit, cmd, 1);
    i2c.read(addr8bit, cmd, 2);

    float tmp = (float((cmd[0]<<8) | cmd[1]) / 256.0);
    // RawSerial::printf() does not support floating point...
    pc.printf("Temp = %d.%d\n", int(tmp), int(tmp * 10.0f) % 10);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
