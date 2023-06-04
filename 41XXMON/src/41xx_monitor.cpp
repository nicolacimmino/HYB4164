#include "41xx_monitor.h"

char printBuffer[PRINT_BUFFER_SIZE];
byte rxBuffer[RX_BUFFER_SIZE];
uint8_t memoryLayout = MEM_LAYOUT_BYTE;

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

  switch (command)
  {
  case CMD_MEMORY:
    dumpMemory();
    break;
  case CMD_HELP:
    printMessage(MESSAGE_HELP_IX);
    break;
  case CMD_WRITE:
    writeMemory();
    break;
  case CMD_LAYOUT:
    switchMemoryLayout();
    break;
  default:
    Serial.println(F("UNKNOWN COMMAND."));
    return RES_ERR;
  }

  return RES_OK;
}

char *getCharParam()
{
  return strtok(NULL, " \r\n");
}

int getIntParam(int defaultValue)
{
  char *token = strtok(NULL, " ");
  if (token != NULL)
  {
    return strtoul(token, NULL, 16);
  }

  return defaultValue;
}

void switchMemoryLayout()
{
  char *arg = getCharParam();

  if (strcmp(arg, "BIT") == 0)
  {
    memoryLayout = MEM_LAYOUT_BIT;
  }
  else if (strcmp(arg, "NIBBLE") == 0)
  {
    memoryLayout = MEM_LAYOUT_NIBBLE;
  }
  else if (strcmp(arg, "BYTE") == 0)
  {
    memoryLayout = MEM_LAYOUT_BYTE;
  }
  else if (strcmp(arg, "WORD16") == 0)
  {
    memoryLayout = MEM_LAYOUT_WORD16;
  }
  else if (strcmp(arg, "WORD32") == 0)
  {
    memoryLayout = MEM_LAYOUT_WORD32;
  }
  else if (strcmp(arg, "WORD64") == 0)
  {
    memoryLayout = MEM_LAYOUT_WORD64;
  }
  else
  {
    serialPrintf("Unknown layout: %s", arg);
  }
}
void writeMemory()
{
  byte rxBufferIx = 0;
  char *token;
  int address = getIntParam(0);

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

void dumpMemory()
{
  int start = getIntParam(0);

  for (int address = start - (start % MON_DUMP_PER_LINE); address < EXT_MEM_SIZE; address++)
  {
    if (address % MON_DUMP_PER_LINE == 0)
    {
      serialPrintf("%04X  ", address);
    }

    if (address < start)
    {
      serialPrintf("   ");
      continue;
    }

    switch (memoryLayout)
    {
    case MEM_LAYOUT_BIT:
      serialPrintf("%s%s", dram4164::readBit(address) ? "1" : "0", (((address + 1) % MON_DUMP_PER_LINE) == 0) ? "\r\n" : ".");
      break;
    case MEM_LAYOUT_NIBBLE:
      serialPrintf("%01X%s", dram4164::readNibble(address), (((address + 1) % MON_DUMP_PER_LINE) == 0) ? "\r\n" : ".");
      break;
    case MEM_LAYOUT_BYTE:
      serialPrintf("%02X%s", dram4164::readByte(address), (((address + 1) % MON_DUMP_PER_LINE) == 0) ? "\r\n" : ".");
      break;
    case MEM_LAYOUT_WORD16:
      serialPrintf("%04X%s", dram4164::readWord<uint16_t>(address), (((address + 1) % MON_DUMP_PER_LINE) == 0) ? "\r\n" : ".");
      break;
    case MEM_LAYOUT_WORD32:
      serialPrintf("%08X%s", dram4164::readWord<uint32_t>(address), (((address + 1) % MON_DUMP_PER_LINE) == 0) ? "\r\n" : ".");
      break;
    case MEM_LAYOUT_WORD64:
      serialPrintf("%0FX%s", dram4164::readWord<uint64_t>(address), (((address + 1) % MON_DUMP_PER_LINE) == 0) ? "\r\n" : ".");
      break;
    }

    if (((address + 1) % MON_DUMP_PER_LINE == 0) && (waitForKeyPress() == 'X'))
    {
      return;
    }
  }
}

void printSingleMemoryLocation(int address, bool printNewLine = false)
{
  serialPrintf("%04X  %02X .%s",
               address,
               dram4164::readByte(address),
               printNewLine ? "\r\n" : "");
}

void printMessage(uint8_t messageId)
{
  strcpy_P(printBuffer, (char *)pgm_read_word(&(messages[messageId])));
  Serial.print(printBuffer);
}

void serialPrintf(const char *format, ...)
{
  va_list args;
  va_start(args, format);

  vsprintf(printBuffer, format, args);

  va_end(args);

  Serial.print(printBuffer);
}

char waitForKeyPress()
{
  while (!Serial.available())
    ;

  return toupper(Serial.read());
}