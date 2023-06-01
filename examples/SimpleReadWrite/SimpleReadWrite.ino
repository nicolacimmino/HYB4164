#include <DRAM4164Driver.h>

void setup()
{
    dram4164_begin();
    Serial.begin(9600);
}

void loop()
{
    // Write and read a single bit.
    // Max address is 65535.
    dram4164_writeBit(0, 1);
    Serial.println(dram4164_readBit(0));

    // Write and read a single nibble.
    // Max address is 16383.
    dram4164_writeNibble(0, 0xA);
    Serial.println(dram4164_readNibble(0), 16);

    // Write and read a single byte.
    // Max address is 8191.
    dram4164_writeByte(0, 0xB);
    Serial.println(dram4164_readByte(0), 16);

    // Write and read a single 16-bit word.
    // Max address is 4095.
    dram4164_writeWord(0, (uint16_t)0x5A5A);
    Serial.println(dram4164_readWord<uint16_t>(0), 16);

    // Write and read a single 32-bit word.
    // Max address is 2047.
    dram4164_writeWord(0, (uint32_t)0xA0125A5A);
    Serial.println(dram4164_readWord<uint32_t>(0), 16);

    // Write a single 32-bit word and read it back
    //  as bytes. Note that the numbers are written
    //  as little-endians, so the LSB will be in 0
    //  and the MSB in 3.
    dram4164_writeWord(0, (uint32_t)0xA0125A5A);
    Serial.println(dram4164_readByte(0), 16);
    Serial.println(dram4164_readByte(1), 16);
    Serial.println(dram4164_readByte(2), 16);
    Serial.println(dram4164_readByte(3), 16);

    // Read and write a 256 bits block. This is passed to
    //  and from the functions as as 32-byte array.
    // Max address is 256.
    uint8_t buffer[32];
    memset(buffer, 0xA5, 32);
    dram4164_writeBlock(256, 0, buffer);

    memset(buffer, 0xA5, 32);
    dram4164_readBlock(256, 0, buffer);

    for (uint8_t ix = 0; ix < 32; ix++)
    {
        Serial.println(buffer[ix], 16);
    }

    Serial.println("--------");

    while (1)
        ;
}
