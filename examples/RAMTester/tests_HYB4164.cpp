#include "tests_HYB4164.h"

bool runTest()
{
    setupTestHarness();

    Serial.println(F("\r\n--------- TESTS - START ---------"));

    testWriteZeros();
    testWriteOnes();
    testWriteBitPattern();
    testWriteNibblePattern();
    testWriteBytePattern();
    testWriteWord8Pattern();
    testWriteWord16Pattern();
    testWriteWord32Pattern();
    testWriteWord64Pattern();
    testWriteBlockPattern();

    if (failures > 0)
    {
        sprintf(printBuffer, "FAILED!\r\n%d/%d", failures, asserts);
        Serial.println(printBuffer);
    }

    Serial.println(F("-------------  END  -------------"));
    Serial.println("");

    Serial.println(F("-------- MEASURE - START --------"));
    Serial.println(F("                         T  T/bit"));

    measureWriteBitTime();
    measureWriteNibbleTime();
    measureWriteByteTime();
    measureWriteWord8Time();
    measureWriteWord16Time();
    measureWriteWord32Time();
    measureWriteWord64Time();
    measureWriteBlock32Time();
    measureWriteBlock64Time();
    measureWriteBlock128Time();
    measureWriteBlock256Time();

    measureReadBitTime();
    measureReadNibbleTime();
    measureReadByteTime();
    measureReadWord8Time();
    measureReadWord16Time();
    measureReadWord32Time();
    measureReadWord64Time();
    measureReadBlock32Time();
    measureReadBlock64Time();
    measureReadBlock128Time();
    measureReadBlock256Time();

    if (failures > 0)
    {
        countBadBits();
    }

    Serial.println(F("-------------  END  -------------"));
    Serial.println("");

    return (failures == 0);
}

void powerUpHYB4164()
{
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
        writeByte(ix, ix);
    }
}

void testWriteNibblePattern()
{
    for (uint16_t ix = 0; ix < 16384; ix++)
    {
        writeByte(ix, ix & 0xF);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 16384; ix++)
    {
        if (readByte(ix) != (ix & 0XF))
        {
            match = false;
        }
    }

    reportResult(match, "Write Nibble Pattern");
}

void testWriteBytePattern()
{
    for (uint16_t ix = 0; ix < 8192; ix++)
    {
        writeByte(ix, ix & 0xFF);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 8192; ix++)
    {
        if (readByte(ix) != (ix & 0XFF))
        {
            match = false;
        }
    }

    reportResult(match, "Write Byte Pattern");
}

void testWriteWord8Pattern()
{
    for (uint16_t ix = 0; ix < 8192; ix++)
    {
        writeWord<uint8_t>(ix, (uint8_t)(ix & 0xFF));
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 8192; ix++)
    {
        if (readWord<uint8_t>(ix) != (ix & 0XFF))
        {
            match = false;
        }
    }

    reportResult(match, "Write Word8 Pattern");
}

void testWriteBlockPattern()
{
    uint8_t blockContent[32];
    for (uint8_t ix = 0; ix < 32; ix++)
    {
        blockContent[ix] = ix * 2;
    }

    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeBlock(256, ix, blockContent);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        memset(blockContent, 0, 32);

        readBlock(256, ix, blockContent);

        for (uint8_t iy = 0; iy < 32; iy++)
        {
            if (blockContent[iy] != (iy * 2))
            {
                match = false;
            }
        }
    }

    reportResult(match, "Write Block Pattern");
}

void testWriteWord16Pattern()
{
    for (uint16_t ix = 0; ix < 4096; ix++)
    {
        writeWord<uint16_t>(ix, (uint16_t)(ix & 0xFFFF));
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 4096; ix++)
    {
        if (readWord<int16_t>(ix) != (ix & 0XFFFF))
        {
            match = false;
        }
    }

    reportResult(match, "Write Word16 Pattern");
}

void testWriteWord32Pattern()
{
    for (uint16_t ix = 0; ix < 2048; ix++)
    {
        writeWord<uint32_t>(ix, (uint32_t)ix);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 2048; ix++)
    {
        if (readWord<uint32_t>(ix) != ix)
        {
            match = false;
        }
    }

    reportResult(match, "Write Word32 Pattern");
}

void testWriteWord64Pattern()
{
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeWord<uint64_t>(ix, (uint64_t)ix);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        if (readWord<uint64_t>(ix) != ix)
        {
            match = false;
        }
    }

    reportResult(match, "Write Word64 Pattern");
}

void testWriteZeros()
{
    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        writeBit(ix, false);
    }

    bool match = true;
    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        if (readBit(ix) != false)
        {
            match = false;
        }
    }

    reportResult(match, "Write Zeros");
}

void testWriteOnes()
{
    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        writeBit(ix, true);
    }

    bool match = true;
    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        if (readBit(ix) != true)
        {
            match = false;
        }
    }

    reportResult(match, "Write Ones");
}

void testWriteBitPattern()
{
    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        writeBit(ix, ix & 0x1);
    }

    bool match = true;
    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        if (readBit(ix) != (ix & 0x1 == 0x1))
        {
            match = false;
        }
    }

    reportResult(match, "Write Bit Pattern");
}

void measureWriteNibbleTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeNibble(ix, 1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 4, "uS", "Write Nibble");
}

void measureWriteByteTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeByte(ix, 1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 8, "uS", "Write Byte");
}

void measureWriteWord8Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeWord<uint8_t>(ix, (uint8_t)1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 8, "uS", "Write Word8");
}

void measureWriteWord16Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 512; ix++)
    {
        writeWord<uint16_t>(ix, (uint16_t)1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 512;

    reportDoubleNumericResult(timeuS, timeuS / 16, "uS", "Write Word16");
}

void measureWriteWord32Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeWord<uint32_t>(ix, (uint32_t)1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 32, "uS", "Write Word32");
}

void measureWriteWord64Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeWord<uint64_t>(ix, (uint64_t)1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 64, "uS", "Write Word64");
}

void measureWriteBlock32Time()
{
    uint8_t buffer[4];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeBlock(128, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 32, "uS", "Write Block32");
}

void measureWriteBlock64Time()
{
    uint8_t buffer[8];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeBlock(128, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 64, "uS", "Write Block64");
}

void measureWriteBlock128Time()
{
    uint8_t buffer[16];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeBlock(128, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 128, "uS", "Write Block128");
}

void measureWriteBlock256Time()
{
    uint8_t buffer[32];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeBlock(256, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 256, "uS", "Write Block256");
}

void measureReadBlock32Time()
{
    uint8_t buffer[4];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        readBlock(64, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 32, "uS", "Read Block32");
}

void measureReadBlock64Time()
{
    uint8_t buffer[8];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        readBlock(64, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 64, "uS", "Read Block64");
}

void measureReadBlock128Time()
{
    uint8_t buffer[16];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        readBlock(128, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 128, "uS", "Read Block128");
}

void measureReadBlock256Time()
{
    uint8_t buffer[32];
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        readBlock(256, ix, buffer);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 256, "uS", "Read Block256");
}

void measureWriteBitTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeBit(ix, 1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS, "uS", "Write Bit");
}

void measureReadBitTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readBit(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS, "uS", "Read Bit");
}

void measureReadNibbleTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readNibble(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 4, "uS", "Read Nibble");
}

void measureReadByteTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readByte(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 8, "uS", "Read Byte");
}

void measureReadWord8Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readWord<uint8_t>(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 8, "uS", "Read Word8");
}

void measureReadWord16Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readWord<uint16_t>(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportDoubleNumericResult(timeuS, timeuS / 16, "uS", "Read Word16");
}

void measureReadWord32Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        readWord<uint32_t>(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 32, "uS", "Read Word32");
}

void measureReadWord64Time()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        readWord<uint64_t>(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 256;

    reportDoubleNumericResult(timeuS, timeuS / 64, "uS", "Read Word64");
}

void countBadBits()
{
    uint32_t badCount = 0;

    for (uint32_t ix = 0; ix < 65535; ix++)
    {
        writeBit(ix, true);
        if (readBit(ix) != true)
        {
            badCount++;
            continue;
        }

        writeBit(ix, false);
        if (readBit(ix) != false)
        {
            badCount++;
        }
    }

    reportNumericResult(badCount, "", "Bad Bits");
}
