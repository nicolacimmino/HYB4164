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
    Serial.println(F("------------- START -------------"));

    setupTestHarness();
    powerUpHYB4164();

    Serial.println(F("Power up                       OK"));

    testWritePattern();
    testWriteZeros();
    testWriteOnes();

    releaseHYB4164();

    Serial.println(F("Release DUT                    OK"));

    if (failures > 0)
    {
        sprintf(printBuffer, "FAILED!\r\n%d/%d", failures, asserts);
        Serial.println(printBuffer);
    }

    Serial.println(F("-------------  END  -------------"));
    Serial.println("");

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
        refreshRow();
    }
}

void writeAddressBus(uint8_t address)
{
    digitalWrite(PIN_A0, address & 0x1);
    digitalWrite(PIN_A1, (address >> 1) & 0x1);
    digitalWrite(PIN_A2, (address >> 2) & 0x1);
    digitalWrite(PIN_A3, (address >> 3) & 0x1);
    digitalWrite(PIN_A4, (address >> 4) & 0x1);
    digitalWrite(PIN_A5, (address >> 5) & 0x1);
    digitalWrite(PIN_A6, (address >> 6) & 0x1);
    digitalWrite(PIN_A7, (address >> 7) & 0x1);
}

void writeMemory(uint16_t address, bool value)
{
    writeAddressBus(address & 0xFF);
    digitalWrite(PIN_RAS_N, LOW);

    digitalWrite(PIN_WE_N, LOW);

    digitalWrite(PIN_DI, value);

    writeAddressBus((address >> 8) & 0xFF);
    digitalWrite(PIN_CAS_N, LOW);

    digitalWrite(PIN_WE_N, HIGH);
    digitalWrite(PIN_CAS_N, HIGH);
    digitalWrite(PIN_RAS_N, HIGH);
}

bool readMemory(uint16_t address)
{
    writeAddressBus(address & 0xFF);
    digitalWrite(PIN_RAS_N, LOW);

    writeAddressBus((address >> 8) & 0xFF);
    digitalWrite(PIN_CAS_N, LOW);

    bool value = (digitalRead(PIN_DO) == HIGH);

    digitalWrite(PIN_CAS_N, HIGH);
    digitalWrite(PIN_RAS_N, HIGH);

    return value;
}

void writeByte(uint16_t address, uint8_t value)
{
    for (uint8_t ix = 0; ix < 8; ix++)
    {
        writeMemory((address << 3) + ix, (value >> ix) & 0x1);
    }

    refreshRow();
}

uint8_t readByte(uint16_t address)
{
    uint8_t value = 0;

    for (uint8_t ix = 0; ix < 8; ix++)
    {
        value = value | ((readMemory((address << 3) + ix) ? 1 : 0) << ix);
    }

    refreshRow();

    return value;
}

void testWritePattern()
{
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeByte(ix, ix);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        if (readByte(ix) != ix)
        {
            match = false;
        } 
    }

    reportResult(match, "Write Pattern");
}

void testWriteZeros()
{
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeByte(ix, 0);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        if (readByte(ix) != 0)
        {
            match = false;
        }
        refreshRow();
    }

    reportResult(match, "Write Zeros");
}

void testWriteOnes()
{
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        writeByte(ix, 1);
    }

    bool match = true;
    for (uint16_t ix = 0; ix < 256; ix++)
    {
        if (readByte(ix) != 1)
        {
            match = false;
        }
        refreshRow();
    }

    reportResult(match, "Write Ones");
}



uint8_t refreshRowAddress = 0;

void refreshRow()
{
    writeAddressBus(refreshRowAddress);
    digitalWrite(PIN_RAS_N, LOW);
    digitalWrite(PIN_RAS_N, HIGH);
    refreshRowAddress++;
}