#include "mbed.h"

// use fully qualified class name to resolve ambiguities with
// Arduino's own global "SPI" object
mbed::SPI spi(SPI_MOSI, SPI_MISO, SPI_SCK);
DigitalOut cs(D10);

RawSerial pc(USBTX, USBRX);

void setup() {
    // Chip must be deselected
    cs = 1;

    // Setup the spi for 8 bit data, high steady state clock, second
    // edge capture, with a 1MHz clock rate
    spi.format(8, 3);
    spi.frequency(1000000);

    // Select the device by seting chip select low
    cs = 0;

    // Send 0x8f, the command to read the WHOAMI register
    spi.write(0x8f);

    // Send a dummy byte to receive the contents of the WHOAMI
    // register
    int whoami = spi.write(0x00);
    pc.printf("WHOAMI register = 0x%02x\n", whoami);

    // Deselect the device
    cs = 1;
}

void loop() {
}

#ifndef ARDUINO
int main() {
    setup();
    for (;;) {
        loop();
    }
}
#endif
