
#ifndef __HYB4164_DRIVER_H__
#define __HYB4164_DRIVER_H__

#include <Arduino.h>
#include <digitalWriteFast.h>

#define PIN_NC1 2
#define PIN_DI 3
#define PIN_WE_N 4
#define PIN_RAS_N 5
#define PIN_A0 6
#define PIN_A2 7
#define PIN_A1 8
#define PIN_VCC 9
#define PIN_A7 10
#define PIN_A5 11
#define PIN_A4 12
#define PIN_A3 13
#define PIN_A6 A0
#define PIN_DO A1
#define PIN_CAS_N A2
#define PIN_GND A3

void setAddress(uint8_t address);
void writeBit(uint16_t address, bool value);
bool readBit(uint16_t address);
void writeNibble(uint16_t address, uint8_t value);
uint8_t readNibble(uint16_t address);
void readBlock(uint16_t blockSize, uint16_t address, uint8_t *value);
void writeBlock(uint16_t blockSize, uint16_t address, uint8_t *value);

template <typename T>
void writeWord(uint16_t address, T value)
{
    uint8_t wordSize = sizeof(T) * 8;
    uint8_t bits = 0;
    while (wordSize = wordSize >> 1)
    {
        bits++;
    }
    wordSize = sizeof(T) * 8;

    setAddress((address >> (8 - bits)) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    for (uint8_t ix = 0; ix < wordSize; ix++)
    {
        digitalWriteFast(PIN_WE_N, LOW);

        digitalWriteFast(PIN_DI, (value >> ix) & 0x1);

        setAddress((address << bits) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        digitalWriteFast(PIN_WE_N, HIGH);
        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);
}

constexpr void (*writeByte)(uint16_t address, uint8_t value) = &writeWord<uint8_t>;

template <typename T>
T readWord(uint16_t address)
{
    uint8_t wordSize = sizeof(T) * 8;
    uint8_t bits = 0;
    while (wordSize = wordSize >> 1)
    {
        bits++;
    }
    wordSize = sizeof(T) * 8;

    setAddress((address >> (8 - bits)) & 0xFF);
    digitalWriteFast(PIN_RAS_N, LOW);

    uint64_t value = 0;
    for (uint8_t ix = 0; ix < wordSize; ix++)
    {
        setAddress((address << bits) | ix);
        digitalWriteFast(PIN_CAS_N, LOW);

        delayMicroseconds(2);
        value = value | ((digitalReadFast(PIN_DO) ? 1 : 0) << ix);

        digitalWriteFast(PIN_CAS_N, HIGH);
    }

    digitalWriteFast(PIN_RAS_N, HIGH);

    return value;
}

constexpr uint8_t (*readByte)(uint16_t address) = &readWord<uint8_t>;

#endif
