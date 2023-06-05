#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <Arduino.h>
#include <DRAM4164Driver.h>
#include "messages.h"

#define PRINT_BUFFER_SIZE 128
#define RX_BUFFER_SIZE 32
#define EXT_MEM_SIZE 8152

#define RES_OK 0
#define RES_ERR 1
#define RES_LEAVE_MONITOR 2

#define CMD_MEMORY 0
#define CMD_HELP 1
#define CMD_WRITE 2
#define CMD_TEST 3
#define CMD_LAYOUT 4
#define CMD_MAX 5

#define TERMINAL_KEY_BACKSPACE 0x7F

#define MON_MAX_INACTIVE_MS 30000

#define MON_DUMP_PER_LINE 8

extern byte rxBuffer[];
extern char printBuffer[PRINT_BUFFER_SIZE];

int getIntParam(int defaultValue);
char* getCharParam();

uint8_t processCommand();
void enterMonitor();
void dumpMemory();
void writeMemory();
void switchMemoryLayout();

void printSingleMemoryLocation(int address, bool printNewLine = false);
void printMessage(uint8_t messageId);

const char commands[] = "MHWTL";

void serialPrintf(const char *format, ...);
char waitForKeyPress();

#define MEM_LAYOUT_BIT 1
#define MEM_LAYOUT_NIBBLE 4
#define MEM_LAYOUT_BYTE 8
#define MEM_LAYOUT_WORD16 16
#define MEM_LAYOUT_WORD32 32
#define MEM_LAYOUT_WORD64 64

extern uint8_t memoryLayout;

#endif
