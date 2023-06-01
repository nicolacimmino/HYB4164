#include "DRAM4164Driver.h"

void dram4164_begin() {
    pinMode(PIN_VCC, OUTPUT);
    pinMode(PIN_GND, OUTPUT);

    digitalWrite(PIN_VCC, HIGH);
    digitalWrite(PIN_GND, LOW);

    pinMode(PIN_RAS_N, OUTPUT);
    pinMode(PIN_CAS_N, OUTPUT);
    pinMode(PIN_WE_N, OUTPUT);
    pinMode(PIN_A0, OUTPUT);
    pinMode(PIN_A1, OUTPUT);
    pinMode(PIN_A2, OUTPUT);
    pinMode(PIN_A3, OUTPUT);
    pinMode(PIN_A4, OUTPUT);
    pinMode(PIN_A5, OUTPUT);
    pinMode(PIN_A6, OUTPUT);
    pinMode(PIN_A7, OUTPUT);
    pinMode(PIN_DI, OUTPUT);
    pinMode(PIN_DO, INPUT);

    digitalWrite(PIN_RAS_N, HIGH);
    digitalWrite(PIN_CAS_N, HIGH);

    delay(100);

    for (uint16_t ix = 0; ix < 256; ix++)
    {
        dram4164_writeBit(ix, 1);
    }
}

void dram4164_setAddress(uint8_t address)
{
    digitalWriteFast(PIN_A0, address & 0x1);
    digitalWriteFast(PIN_A1, (address >> 1) & 0x1);
    digitalWriteFast(PIN_A2, (address >> 2) & 0x1);
    digitalWriteFast(PIN_A3, (address >> 3) & 0x1);
    digitalWriteFast(PIN_A4, (address >> 4) & 0x1);
    digitalWriteFast(PIN_A5, (address >> 5) & 0x1);
    digitalWriteFast(PIN_A6, (address >> 6) & 0x1);
    digitalWriteFast(PIN_A7, (address >> 7) & 0x1);
}

void dram4164_writeBit(uint16_t address, bool value)
{
    dram4164_setAddress((address >> 8) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    digitalWriteFast(PIN_WE_N, LOW);

    digitalWriteFast(PIN_DI, value ? HIGH : LOW);

    dram4164_setAddress(address & 0xFF);
    digitalWriteFast(PIN_CAS_N, LOW);

    digitalWriteFast(PIN_WE_N, HIGH);
    digitalWriteFast(PIN_CAS_N, HIGH);
    digitalWriteFast(PIN_RAS_N, HIGH);
}

bool dram4164_readBit(uint16_t address)
{
    dram4164_setAddress((address >> 8) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    dram4164_setAddress(address & 0xFF);
    digitalWriteFast(PIN_CAS_N, LOW);

    delayMicroseconds(2);
    bool value = (digitalReadFast(PIN_DO) == HIGH);

    digitalWriteFast(PIN_CAS_N, HIGH);
    digitalWriteFast(PIN_RAS_N, HIGH);

    return value;
}

void dram4164_writeNibble(uint16_t address, uint8_t value)
{
    dram4164_setAddress((address >> 6) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint8_t ix = 0; ix < 4; ix++)
    {
        digitalWriteFast(PIN_WE_N, LOW);

        digitalWriteFast(PIN_DI, (value >> ix) & 0x1);

        dram4164_setAddress((address << 2) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        digitalWriteFast(PIN_WE_N, HIGH);
        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}

uint8_t dram4164_readNibble(uint16_t address)
{
    dram4164_setAddress((address >> 6) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    uint8_t value = 0;
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        dram4164_setAddress((address << 2) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        delayMicroseconds(2);
        value = value | ((digitalReadFast(PIN_DO) ? 1 : 0) << ix);

        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);

    return value;
}

void dram4164_writeBlock(uint16_t blockSize, uint16_t address, uint8_t *value)
{
    uint8_t bits = 0;
    uint16_t tmpBlockSize = blockSize;
    while (tmpBlockSize = tmpBlockSize >> 1)
    {
        bits++;
    }

    dram4164_setAddress((address >> (8 - bits)) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint16_t ix = 0; ix < blockSize; ix++)
    {
        digitalWriteFast(PIN_WE_N, LOW);

        uint8_t bitValue = (value[ix >> 3] >> (ix & 0b111)) & 0b1;

        digitalWriteFast(PIN_DI, bitValue);

        dram4164_setAddress((address << bits) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        digitalWriteFast(PIN_WE_N, HIGH);
        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}

void dram4164_readBlock(uint16_t blockSize, uint16_t address, uint8_t *value)
{
    uint8_t bits = 0;
    uint16_t tmpBlockSize = blockSize;
    while (tmpBlockSize = tmpBlockSize >> 1)
    {
        bits++;
    }

    memset(value, 0, (blockSize >> 3));

    dram4164_setAddress((address >> (8 - bits)) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint16_t ix = 0; ix < blockSize; ix++)
    {
        dram4164_setAddress((address << bits) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        delayMicroseconds(2);
        value[ix >> 3] = value[ix >> 3] | ((digitalReadFast(PIN_DO) ? 1 : 0) << (ix & 0b111));

        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}