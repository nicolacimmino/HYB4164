
#ifndef __TESTS_MC14500__
#define __TESTS_MC14500__

#include <Arduino.h>
#include "test_harness.h"
#include "HYB4164_driver.h"

bool runTest();

void powerUpHYB4164();

void testWriteBytePattern();
void testWriteBitPattern();
void testWriteZeros();
void testWriteOnes();
void measureWriteBitTime();
void measureWriteByteTime();
void measureReadBitTime();
void measureReadByteTime();
void countBadBits();

#endif