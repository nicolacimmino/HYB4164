#ifndef __ASSEMBLER_H__
#define __ASSEMBLER_H__

#include <Arduino.h>
#include <DRAM4164Driver.h>
#include "messages.h"

#define PRINT_BUFFER_SIZE 256
#define RX_BUFFER_SIZE 32
#define EXT_MEM_SIZE 8152

#define RES_OK 0
#define RES_ERR 1
#define RES_LEAVE_MONITOR 2

#define CMD_MEMORY 0
#define CMD_HELP 1
#define CMD_WRITE 2
#define CMD_TEST 3
#define CMD_MAX 4

#define TERMINAL_KEY_BACKSPACE 0x7F

#define MON_MAX_INACTIVE_MS 30000

#define MON_DUMP_PER_LINE 8

extern byte rxBuffer[];
extern char printBuffer[PRINT_BUFFER_SIZE];

uint8_t processCommand();
void enterMonitor();
void dumpMemory(int start);
void writeMemory(int address);
void printSingleMemoryLocation(int address, bool printNewLine = false);
//void test(int loop);
void printMessage(uint8_t messageId);

const char commands[] = "MHWT";

void serialPrintf(const char *format, ...);
char waitForKeyPress();

#endif
