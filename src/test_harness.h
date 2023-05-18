
#ifndef __TEST_HARNESS_H__
#define __TEST_HARNESS_H__

#include <Arduino.h>

#include "config.h"

#define PIN_BUTTON A6
#define PIN_LED_RED A5
#define PIN_LED_GREEN A4

#define DUT16_PIN_1 2
#define DUT16_PIN_2 3
#define DUT16_PIN_3 4
#define DUT16_PIN_4 5
#define DUT16_PIN_5 6
#define DUT16_PIN_6 7
#define DUT16_PIN_7 8
#define DUT16_PIN_8 9
#define DUT16_PIN_9 10
#define DUT16_PIN_10 11
#define DUT16_PIN_11 12
#define DUT16_PIN_12 13
#define DUT16_PIN_13 A0
#define DUT16_PIN_14 A1
#define DUT16_PIN_15 A2
#define DUT16_PIN_16 A3

#define DUT14_PIN_1 2
#define DUT14_PIN_2 3
#define DUT14_PIN_3 4
#define DUT14_PIN_4 5
#define DUT14_PIN_5 6
#define DUT14_PIN_6 7
#define DUT14_PIN_7 8
#define DUT14_PIN_8 11
#define DUT14_PIN_9 12
#define DUT14_PIN_10 13
#define DUT14_PIN_11 A0
#define DUT14_PIN_12 A1
#define DUT14_PIN_13 A2
#define DUT14_PIN_14 A3

#define PRINT_BUFFER_SIZE 80

extern char printBuffer[PRINT_BUFFER_SIZE];
extern uint8_t failures;
extern uint8_t asserts;

bool isButtonPressed();
void setupTestHarness();
void setLine(byte line, byte level);
byte readLine(byte line);
void expect(byte line, byte value, const char *message);
void reportResult(bool result, const char *message);
void reportNumericResult(uint16_t result, const char *unit, const char *message);

#endif