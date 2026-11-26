#include "cmd/uart_print.h"

__attribute__((section(".text.cmd.uart_print")))
void uart_putc(const char c) {
    while (REG32(UART0_TFR) & (0x01 << 5)); 
    REG32(UART0_DR) = c;
}

__attribute__((section(".text.cmd.uart_print")))
void uart_putnl(){
    while (REG32(UART0_TFR) & (0x01 << 5)); 
    REG32(UART0_DR) = '\r';
    while (REG32(UART0_TFR) & (0x01 << 5)); 
    REG32(UART0_DR) = '\n';
}

__attribute__((section(".text.cmd.uart_print")))
void uart_puts(const char *string, uint8 new_line) {
    // new_line is a bool - if true - puts new line on the end of the string 
    while(*string != '\0'){
        uart_putc(*string);
        string++;
    }
    if (new_line > 0) uart_putnl();
}
