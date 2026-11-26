#include "hw/hw_uart.h"
#include "cmd/uart_print.h"
#include "cmd/cmd.h"


__attribute__((section(".text.isr.uart0_isr")))
void uart0_isr() {
    // read the char from uart
    char c = REG32(UART0_DR);
    if (buff_ptr < sizeof(uart_buff) -1) {
        if (c == '\r') {
            uart_putnl();
            uart_buff[buff_ptr] = '\0';
            buff_ptr = 0;
            command_ready = 1;
        } else {
            uart_putc(c);
            uart_buff[buff_ptr] = c;
            buff_ptr += 1;
        }
    } else {
        buff_ptr = 0;
        uart_putnl();
    } 
    REG32(UART0_ICR) = (0x01 << 4);
}
