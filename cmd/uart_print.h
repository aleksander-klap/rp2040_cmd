#ifndef UART_PRINT_H
#define UART_PRINT_H

#include "globals.h"
#include "hw/hw_uart.h"

__attribute__((section(".text.cmd.uart_print")))
void uart_putc(const char c);

__attribute__((section(".text.cmd.uart_print")))
void uart_putnl();

__attribute__((section(".text.cmd.uart_print")))
void uart_puts(const char *string, uint8 new_line);

#endif
