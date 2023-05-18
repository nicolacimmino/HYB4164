#include "tests_HYB4164.h"

byte dut_pins[] = {
    PIN_NC1,
    PIN_DI,
    PIN_WE_N,
    PIN_RAS_N,
    PIN_A0,
    PIN_A2,
    PIN_A1,
    PIN_VCC,
    PIN_A7,
    PIN_A5,
    PIN_A4,
    PIN_A3,
    PIN_A6,
    PIN_DO,
    PIN_CAS_N,
    PIN_GND,
};

bool runTest()
{
    setupTestHarness();
    powerUpHYB4164();

    Serial.println(F("--------- TESTS - START ---------"));

    testWriteBytePattern();
    testWriteBitPattern();
    testWriteZeros();
    testWriteOnes();

    if (failures > 0)
    {
        sprintf(printBuffer, "FAILED!\r\n%d/%d", failures, asserts);
        Serial.println(printBuffer);
    }

    Serial.println(F("-------------  END  -------------"));
    Serial.println("");

    Serial.println(F("-------- MEASURE - START --------"));

    measureWriteByteTime();
    measureWriteBitTime();
    measureReadByteTime();
    measureReadBitTime();
    countBadBits();

    Serial.println(F("-------------  END  -------------"));
    Serial.println("");

    releaseHYB4164();

    return (failures == 0);
}

void releaseHYB4164()
{
    for (uint8_t ix = 0; ix < DUT_PINS_COUNT; ix++)
    {
        pinMode(dut_pins[ix], INPUT);
    }
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

void measureWriteByteTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeByte(ix, 1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportNumericResult(timeuS, "uS", "Write Byte Time");
}

void measureWriteBitTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        writeBit(ix, 1);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportNumericResult(timeuS, "uS", "Write Bit Time");
}

void measureReadBitTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readBit(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportNumericResult(timeuS, "uS", "Read Bit Time");
}

void measureReadByteTime()
{
    unsigned long startTime = millis();
    for (uint16_t ix = 0; ix < 1024; ix++)
    {
        readByte(ix);
    }
    unsigned long timeuS = (1000 * (millis() - startTime)) / 1024;

    reportNumericResult(timeuS, "uS", "Read Byte Time");
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
