
#ifndef __TESTS_MC14500__
#define __TESTS_MC14500__

#include <Arduino.h>
#include "test_harness.h"

#define DUT_PINS_COUNT 16

#define PIN_NC1 DUT16_PIN_1
#define PIN_DI DUT16_PIN_2
#define PIN_WE_N DUT16_PIN_3
#define PIN_RAS_N DUT16_PIN_4
#define PIN_A0 DUT16_PIN_5
#define PIN_A2 DUT16_PIN_6
#define PIN_A1 DUT16_PIN_7
#define PIN_VCC DUT16_PIN_8
#define PIN_A7 DUT16_PIN_9
#define PIN_A5 DUT16_PIN_10
#define PIN_A4 DUT16_PIN_11
#define PIN_A3 DUT16_PIN_12
#define PIN_A6 DUT16_PIN_13
#define PIN_DO DUT16_PIN_14
#define PIN_CAS_N DUT16_PIN_15
#define PIN_GND DUT16_PIN_16

bool runTest();

void releaseHYB4164();
void powerUpHYB4164();

void writeAddressBus(uint8_t address);
void writeMemory(uint16_t address, bool value);
bool readMemory(uint16_t address);
void writeByte(uint16_t address, uint8_t value);
uint8_t readByte(uint16_t address);

void testWritePattern();
void testWriteZeros();
void testWriteOnes();
void measureWriteBitTime();
void measureWriteByteTime();

void refreshRow();

#endif