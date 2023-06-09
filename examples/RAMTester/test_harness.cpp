#include "test_harness.h"

uint8_t failures = 0;
uint8_t asserts = 0;

void setupTestHarness()
{
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    pinMode(PIN_LED_GREEN, OUTPUT);
    pinMode(PIN_LED_RED, OUTPUT);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    failures = 0;
    asserts = 0;
}

void setLine(byte line, byte level)
{
    pinMode(line, OUTPUT);
    digitalWrite(line, level);
}

byte readLine(byte line)
{    
    pinMode(line, INPUT);

    return digitalRead(line);
}

void expect(byte line, byte value, const char *message)
{
    bool result = readLine(line) == value;

    sprintf(printBuffer, "%-30s %s\r\n", message, (result ? "OK" : "FAIL"));
    Serial.print(printBuffer);

    asserts++;
    if (!result)
    {
        failures++;
    }
}

void reportResult(bool result, const char *message)
{
    sprintf(printBuffer, "%-30s %s\r\n", message, result ? "OK" : "FAIL");
    Serial.print(printBuffer);

    digitalWrite(PIN_LED_GREEN, result);
    digitalWrite(PIN_LED_RED, !result);
    delay(300);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_RED, LOW);
    
    asserts++;
    if (!result)
    {
        failures++;
    }
}

void reportNumericResult(uint32_t result, const char *unit, const char *message)
{
    sprintf(printBuffer, "%-19s %10lu %s\r\n", message, result, unit);
    Serial.print(printBuffer);  
}

void reportDoubleNumericResult(uint32_t resultA, uint32_t resultB, const char *unit, const char *message) {
    sprintf(printBuffer, "%-18s %7lu %3lu %s\r\n", message, resultA, resultB, unit);
    Serial.print(printBuffer);  
}

bool isButtonPressed()
{
    return analogRead(PIN_BUTTON) < 5;
}
