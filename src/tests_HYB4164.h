
#ifndef __TESTS_MC14500__
#define __TESTS_MC14500__

#include <Arduino.h>
#include "test_harness.h"
#include "HYB4164_driver.h"

bool runTest();

void powerUpHYB4164();

void testWriteWord8Pattern();
void testWriteWord16Pattern();
void testWriteWord32Pattern();
void testWriteWord64Pattern();
void testWriteNibblePattern();
void testWriteBytePattern();
void testWriteBitPattern();
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
void measureWriteWord8Time();
void measureWriteWord16Time();
void measureWriteWord32Time();
void measureWriteWord64Time();
void countBadBits();

#endif