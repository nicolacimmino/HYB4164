
#ifndef __DRIVER_H__
#define __DRIVER_H__

#include <Arduino.h>
#include <digitalWriteFast.h>

namespace dram4164 {
    void setAddress(uint8_t address);

    void begin();
    void writeBit(uint16_t address, bool value);
    bool readBit(uint16_t address);
    void writeNibble(uint16_t address, uint8_t value);
    uint8_t readNibble(uint16_t address);
    void readBlock(uint16_t blockSize, uint16_t address, uint8_t * value);
    void writeBlock(uint16_t blockSize, uint16_t address, uint8_t * value);

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
};

#endif
