#include "mbed.h"

// Scan for standard 7-bit addresses

I2C i2c(I2C_SDA, I2C_SCL);

RawSerial pc(USBTX, USBRX);

void setup() {
}

void loop() {
    pc.printf("Scanning for I2C devices...\r\n");

    int nfound = 0;
    for (int address = 0; address != 128; ++address) {
        if (i2c.write(address << 1, 0, 0) == 0) {
            pc.printf("0x%x ACK\r\n", address);
            ++nfound;
        } else {
            // pc.printf("0x%x NAK\r\n", address);
        }

    }

    pc.printf("%d devices found\r\n", nfound);

    wait(5);
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
