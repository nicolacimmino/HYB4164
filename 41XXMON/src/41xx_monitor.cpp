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
    serialPrintf("UNKNOWN LAYOUT: %s", arg);
  }

  arg = getCharParam();

  if (arg != NULL)
  {
    if (strcmp(arg, "BIG"))
    {
      dram4164::endianess == BIG_ENDIAN;
    }
    else if (strcmp(arg, "LITTLE"))
    {
      dram4164::endianess == LITTLE_ENDIAN;
    }
    else
    {
      serialPrintf("UNKNOWN ENDIANESS: %s", arg);
    }
  }
}

void writeMemory()
{
  byte rxBufferIx = 0;
  char *token;
  int address = getIntParam(0);

  printValue(address, true);
  Serial.print(".");

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
          storeValue(address, strtoul(token, NULL, 16));
        }

        rxBufferIx = 0;

        address = (address + 1) % EXT_MEM_SIZE;

        printValue(address, true);
        Serial.print(".");

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
    printValue(address, (address % MON_DUMP_PER_LINE == 0), true);

    if (((address + 1) % MON_DUMP_PER_LINE == 0) && (waitForKeyPress() == 'X'))
    {
      return;
    }
  }
}

void storeValue(int address, uint64_t value)
{
  switch (memoryLayout)
  {
  case MEM_LAYOUT_BIT:
    dram4164::writeBit(address, value & 0b1);
    break;
  case MEM_LAYOUT_NIBBLE:
    dram4164::writeNibble(address, value & 0b1111);
    break;
  case MEM_LAYOUT_BYTE:
    dram4164::writeByte(address, value & 0xFF);
    break;
  case MEM_LAYOUT_WORD16:
    dram4164::writeWord<uint16_t>(address, value & 0xFFFF);
    break;
  case MEM_LAYOUT_WORD32:
    dram4164::writeWord<uint32_t>(address, value & 0xFFFFFFFF);
    break;
  case MEM_LAYOUT_WORD64:
    dram4164::writeWord<uint64_t>(address, value);
    break;
  default:
    Serial.print("UNKNOWN MEMORY LAYOUT: ");
    Serial.println(memoryLayout);
    return;
  }
}

void printValue(int address, bool prefixWithAddress, bool printTailChar)
{
  if (prefixWithAddress)
  {
    serialPrintf("%04X:", address);
  }

  switch (memoryLayout)
  {
  case MEM_LAYOUT_BIT:
    serialPrintf("%s", dram4164::readBit(address) ? "1" : "0");
    break;
  case MEM_LAYOUT_NIBBLE:
    serialPrintf("%01X", dram4164::readNibble(address));
    break;
  case MEM_LAYOUT_BYTE:
    printValueHelper<uint8_t>(address);
    break;
  case MEM_LAYOUT_WORD16:
    printValueHelper<uint16_t>(address);
    break;
  case MEM_LAYOUT_WORD32:
    printValueHelper<uint32_t>(address);
    break;
  case MEM_LAYOUT_WORD64:
    printValueHelper<uint64_t>(address);
    break;
  default:
    Serial.print("UNKNOWN MEMORY LAYOUT: ");
    Serial.println(memoryLayout);
    return;
  }

  if (printTailChar)
  {
    Serial.print(((address + 1) % MON_DUMP_PER_LINE) == 0 ? "\r\n" : ".");
  }
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