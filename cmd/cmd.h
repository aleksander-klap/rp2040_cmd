#ifndef CMD_H
#define CMD_H

#include "globals.h"

#define UART_BUFF_SIZE 20
#define ARGV_MAX 5

extern volatile uint8 command_ready;
extern volatile uint8 buff_ptr;
extern char uart_buff[UART_BUFF_SIZE];

__attribute__((section(".text.cmd.cmd")))
void parse_command();

#endif
