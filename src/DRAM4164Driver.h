
#ifndef __DRAM4164_DRIVER_H__
#define __DRAM4164_DRIVER_H__

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

    void dram4164_setAddress(uint8_t address);

    void dram4164_begin();
    void dram4164_writeBit(uint16_t address, bool value);
    bool dram4164_readBit(uint16_t address);
    void dram4164_writeNibble(uint16_t address, uint8_t value);
    uint8_t dram4164_readNibble(uint16_t address);
    void dram4164_readBlock(uint16_t blockSize, uint16_t address, uint8_t * value);
    void dram4164_writeBlock(uint16_t blockSize, uint16_t address, uint8_t * value);

    template <typename T>
    void dram4164_writeWord(uint16_t address, T value)
    {
        uint8_t wordSize = sizeof(T) * 8;
        uint8_t bits = 0;
        while (wordSize = wordSize >> 1)
        {
            bits++;
        }
        wordSize = sizeof(T) * 8;

        dram4164_setAddress((address >> (8 - bits)) & 0xFF);
        digitalWriteFast(PIN_RAS_N, LOW);

        for (uint8_t ix = 0; ix < wordSize; ix++)
        {
            digitalWriteFast(PIN_WE_N, LOW);

            digitalWriteFast(PIN_DI, (value >> ix) & 0x1);

            dram4164_setAddress((address << bits) | ix);
            digitalWriteFast(PIN_CAS_N, LOW);

            digitalWriteFast(PIN_WE_N, HIGH);
            digitalWriteFast(PIN_CAS_N, HIGH);
        }

        digitalWriteFast(PIN_RAS_N, HIGH);
    }

    constexpr void (*dram4164_writeByte)(uint16_t address, uint8_t value) = &dram4164_writeWord<uint8_t>;

    template <typename T>
    T dram4164_readWord(uint16_t address)
    {
        uint8_t wordSize = sizeof(T) * 8;
        uint8_t bits = 0;
        while (wordSize = wordSize >> 1)
        {
            bits++;
        }
        wordSize = sizeof(T) * 8;

        dram4164_setAddress((address >> (8 - bits)) & 0xFF);
        digitalWriteFast(PIN_RAS_N, LOW);

        uint64_t value = 0;
        for (uint8_t ix = 0; ix < wordSize; ix++)
        {
            dram4164_setAddress((address << bits) | ix);
            digitalWriteFast(PIN_CAS_N, LOW);

            delayMicroseconds(2);
            value = value | ((digitalReadFast(PIN_DO) ? 1 : 0) << ix);

            digitalWriteFast(PIN_CAS_N, HIGH);
        }

        digitalWriteFast(PIN_RAS_N, HIGH);

        return value;
    }

    constexpr uint8_t (*dram4164_readByte)(uint16_t address) = &dram4164_readWord<uint8_t>;

#endif
