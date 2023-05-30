#include "HYB4164_driver.h"

void setAddress(uint8_t address)
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

void writeBit(uint16_t address, bool value)
{
    setAddress((address >> 8) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    digitalWriteFast(PIN_WE_N, LOW);

    digitalWriteFast(PIN_DI, value ? HIGH : LOW);

    setAddress(address & 0xFF);
    digitalWriteFast(PIN_CAS_N, LOW);

    digitalWriteFast(PIN_WE_N, HIGH);
    digitalWriteFast(PIN_CAS_N, HIGH);
    digitalWriteFast(PIN_RAS_N, HIGH);
}

bool readBit(uint16_t address)
{
    setAddress((address >> 8) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    setAddress(address & 0xFF);
    digitalWriteFast(PIN_CAS_N, LOW);

    delayMicroseconds(2);
    bool value = (digitalReadFast(PIN_DO) == HIGH);

    digitalWriteFast(PIN_CAS_N, HIGH);
    digitalWriteFast(PIN_RAS_N, HIGH);

    return value;
}

void writeNibble(uint16_t address, uint8_t value)
{
    setAddress((address >> 6) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint8_t ix = 0; ix < 4; ix++)
    {
        digitalWriteFast(PIN_WE_N, LOW);

        digitalWriteFast(PIN_DI, (value >> ix) & 0x1);

        setAddress((address << 2) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        digitalWriteFast(PIN_WE_N, HIGH);
        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}

uint8_t readNibble(uint16_t address)
{
    setAddress((address >> 6) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    uint8_t value = 0;
    for (uint8_t ix = 0; ix < 4; ix++)
    {
        setAddress((address << 2) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        delayMicroseconds(2);
        value = value | ((digitalReadFast(PIN_DO) ? 1 : 0) << ix);

        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);

    return value;
}

void writeBlock(uint16_t blockSize, uint16_t address, uint8_t *value)
{
    uint8_t bits = 0;
    uint16_t tmpBlockSize = blockSize;
    while (tmpBlockSize = tmpBlockSize >> 1)
    {
        bits++;
    }

    setAddress((address >> (8 - bits)) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint16_t ix = 0; ix < blockSize; ix++)
    {
        digitalWriteFast(PIN_WE_N, LOW);

        uint8_t bitValue = (value[ix >> 3] >> (ix & 0b111)) & 0b1;

        digitalWriteFast(PIN_DI, bitValue);

        setAddress((address << bits) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        digitalWriteFast(PIN_WE_N, HIGH);
        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}

void readBlock(uint16_t blockSize, uint16_t address, uint8_t *value)
{
    uint8_t bits = 0;
    uint16_t tmpBlockSize = blockSize;
    while (tmpBlockSize = tmpBlockSize >> 1)
    {
        bits++;
    }

    memset(value, 0, (blockSize >> 3));

    setAddress((address >> (8 - bits)) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint16_t ix = 0; ix < blockSize; ix++)
    {
        setAddress((address << bits) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        delayMicroseconds(2);
        value[ix >> 3] = value[ix >> 3] | ((digitalReadFast(PIN_DO) ? 1 : 0) << (ix & 0b111));

        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}