// include the Wire library to enable I2C support
#ifdef ARDUINO
#include <Wire.h>
#endif

#include "mbed.h"

// Read temperature from LM75A

I2C i2c(I2C_SDA, I2C_SCL);

RawSerial pc(USBTX, USBRX);

const int addr7bit = 0x48;      // 7 bit I2C address
const int addr8bit = 0x48 << 1; // 8bit I2C address, 0x90

void setup() {
    // write config register (#1), enable "normal" mode
    char data[2] = {0x01, 0x00};
    i2c.write(addr8bit, data, 2);         
}

void loop() {
    // read temperature register (#0)
    unsigned char cmd[2];
    cmd[0] = 0x00;
    i2c.write(addr8bit, cmd, 1);
    i2c.read(addr8bit, cmd, 2);
    float temp = ((cmd[0] << 8) | cmd[1]) / 256.0f;
    // AVR Libc printf() does not support floating point...
    pc.printf("Temp = %d.%02d°C\r\n", int(temp), int(temp * 100) % 100);
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
