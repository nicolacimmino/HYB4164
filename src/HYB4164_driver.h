
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
void writeByte(uint16_t address, uint8_t value);
uint8_t readByte(uint16_t address);

#endif
