#include "41xx_monitor.h"

char printBuffer[PRINT_BUFFER_SIZE];
byte rxBuffer[RX_BUFFER_SIZE];

void enterMonitor()
{
  uint8_t ix = 0;

  dram4164::begin();
  Serial.begin(9600);

  printMessage(MESSAGE_MONITOR_BANNER_IX);

  while (true)
  {
    while (Serial.available())
    {
      rxBuffer[ix] = toupper(Serial.read());

      Serial.print((char)rxBuffer[ix]);

      if (rxBuffer[ix] == TERMINAL_KEY_BACKSPACE && ix > 0)
      {
        ix--;
        continue;
      }

      if (rxBuffer[ix] == '\r')
      {
        Serial.println(F(""));
        processCommand();

        ix = 0;
        Serial.print(F("\r\n" MONITOR_PROMPT));
        continue;
      }

      if (ix < RX_BUFFER_SIZE)
      {
        ix++;
      }
    }
  }
}

uint8_t processCommand()
{
  char *token = strtok(rxBuffer, " ");

  byte command = CMD_MAX;
  for (byte ix = 0; ix < CMD_MAX; ix++)
  {
    if (strncmp(token, commands + ix, 1) == 0)
    {
      command = ix;
      break;
    }
  }

  int p0 = 0;
  int p1 = EXT_MEM_SIZE - 1;

  token = strtok(NULL, " ");
  if (token != NULL)
  {
    p0 = strtoul(token, NULL, 16);
  }

  token = strtok(NULL, " ");
  if (token != NULL)
  {
    p1 = strtoul(token, NULL, 16);
  }

  switch (command)
  {
  case CMD_MEMORY:
    dumpMemory(p0, p1);
    break;
  case CMD_HELP:
    printMessage(MESSAGE_HELP_IX);
    break;
  case CMD_WRITE:
    writeMemory(p0);
    break;
  // case CMD_TEST:
  //   test(p0);
  //   break;
  default:
    Serial.println(F("UNKNOWN COMMAND."));
    return RES_ERR;
  }

  return RES_OK;
}

void test(int loop)
{
  // while (true)
  // {
  //   bool result = runTest();

  //   digitalWrite(PIN_LED_GREEN, result);
  //   digitalWrite(PIN_LED_RED, !result);

  //   delay(2000);

  //   if (!result || loop == 0)
  //   {
  //     return;
  //   }
  // }
}

void writeMemory(int address)
{
  byte rxBufferIx = 0;
  char *token;

  printSingleMemoryLocation(address);

  while (true)
  {

    while (Serial.available())
    {
      rxBuffer[rxBufferIx] = toupper(Serial.read());

      Serial.print((char)rxBuffer[rxBufferIx]);

      if (rxBuffer[rxBufferIx] == TERMINAL_KEY_BACKSPACE && rxBufferIx == 0)
      {
        continue;
      }

      if (rxBuffer[rxBufferIx] == TERMINAL_KEY_BACKSPACE && rxBufferIx > 0)
      {
        rxBufferIx--;
        continue;
      }

      if (rxBuffer[rxBufferIx] == '\r')
      {
        Serial.println(F(""));

        token = strtok(rxBuffer, " ");

        if (strncmp(token, "X", 1) == 0)
        {
          return;
        }

        if (rxBufferIx > 0)
        {
          dram4164::writeByte(address, strtoul(token, NULL, 16));
        }

        rxBufferIx = 0;

        address = (address + 1) % EXT_MEM_SIZE;

        printSingleMemoryLocation(address);

        continue;
      }

      if (rxBufferIx < RX_BUFFER_SIZE)
      {
        rxBufferIx++;
      }
    }
  }
}

void dumpMemory(int start, int end)
{
  for (int address = start - (start % MON_DUMP_PER_LINE); address < end + 1; address++)
  {
    if (address % MON_DUMP_PER_LINE == 0)
    {
      sprintf(printBuffer, "%04X  ", address);
      Serial.print(printBuffer);
    }

    if (address < start)
    {
      Serial.print("   ");
      continue;
    }

    sprintf(printBuffer,
            "%02X%s",
            dram4164::readByte(address),
            ((address % MON_DUMP_PER_LINE) != (MON_DUMP_PER_LINE - 1)) ? "." : "\r\n");
    Serial.print(printBuffer);
  }

  if (end % MON_DUMP_PER_LINE != (MON_DUMP_PER_LINE - 1))
  {
    Serial.println("");
  }
}

void printSingleMemoryLocation(int address, bool printNewLine = false)
{
  sprintf(printBuffer, "%04X  %02X .", address, dram4164::readByte(address));
  Serial.print(printBuffer);

  if (printNewLine)
  {
    Serial.println(F(""));
  }
}

void printMessage(uint8_t messageId)
{
  strcpy_P(printBuffer, (char *)pgm_read_word(&(messages[messageId])));
  Serial.print(printBuffer);
}