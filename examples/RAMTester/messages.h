
#ifndef __MESSAGES_H__
#define __MESSAGES_H__

#include <Arduino.h>
#include "config.h"

#define MESSAGE_HELP_IX 0
#define MESSAGE_BOOTLOADER_BANNER_IX 1
#define MESSAGE_MONITOR_BANNER_IX 2
#define __MONITOR_VERSION_MAJOR__ "0"
#define __MONITOR_VERSION_MINOR__ "1"

#define MONITOR_PROMPT "."

const char messageHelp[] PROGMEM =
    "M [START] [END] DUMP MEMORY\r\n"
    "T [LOOP]        RUN TESTS\r\n"
    "W [START]       WRITE MEMORY\r\n";
    
const char bootloaderBanner[] PROGMEM =
    "HYB4164\r\n";
    
const char monitorBanner[] PROGMEM =
    "HYB4164MON V" __MONITOR_VERSION_MAJOR__ "." __MONITOR_VERSION_MINOR__ "\r\n" MONITOR_PROMPT;

const char *const messages[] PROGMEM = {messageHelp, bootloaderBanner, monitorBanner};

void printMessage(uint8_t messageId);

#endif