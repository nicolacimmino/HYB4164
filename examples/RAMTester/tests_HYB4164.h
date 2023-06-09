
#ifndef __TESTS_MC14500__
#define __TESTS_MC14500__

#include <Arduino.h>
#include "test_harness.h"
#include <4164Driver.h>

bool runTest();

void powerUpHYB4164();

void testWriteWord8Pattern();
void testWriteWord16Pattern();
void testWriteWord32Pattern();
void testWriteWord64Pattern();
void testWriteNibblePattern();
void testWriteBytePattern();
void testWriteBitPattern();
void testWriteBlockPattern();
void testWriteZeros();
void testWriteOnes();
void measureWriteBitTime();
void measureWriteNibbleTime();
void measureWriteByteTime();
void measureReadBitTime();
void measureReadNibbleTime();
void measureReadByteTime();
void measureReadWord8Time();
void measureReadWord16Time();
void measureReadWord32Time();
void measureReadWord64Time();
void measureReadBlock32Time();
void measureReadBlock64Time();
void measureReadBlock128Time();
void measureReadBlock256Time();
void measureWriteWord8Time();
void measureWriteWord16Time();
void measureWriteWord32Time();
void measureWriteWord64Time();
void measureWriteBlock32Time();
void measureWriteBlock64Time();
void measureWriteBlock128Time();
void measureWriteBlock256Time();
void countBadBits();

#endif