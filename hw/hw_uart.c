#include "hw/hw_uart.h"
#include "globals.h"

__attribute__((section(".text.hw.hw_uart")))
// fifos disabled - trigger irq after every rx byte
void hw_uart0_init() {
    // set uart -> enables tx and rx and uart enable
    // sets baudrate -> (idiv=0x06, fdiv=0x21 => BAUD = 115200)
    REG32(UART0_IBDR) = 0x06;
    REG32(UART0_FBDR) = 0x21;
    // config UART in UARTLCR_H
    // word length = 8 bit
    // parity bits - off
    // fifos disabled - uart data buffers are 8bit registers - full after every char and needs reading
    REG32(UART0_LCR_H) = (0x03 << 5);
    // setup uart interrupts
    // clear all pending UART0 interrupts
    REG32(UART0_ICR) = 0x7ff;
    // set UART RX interrupt mask - trigger interrupt when RX data
    REG32(UART0_IMSC) = 0x01 << 4;
    // in UART UART_FLS we can control when to trigger the interrupt - when buffer half full or full etc.
    // but because our buffer is 8bit its always full or empty. Default is 1/2 full.
    // enable UART module
    REG32(UART0_CR) = (0x01 << 9) | (0x01 << 8) | (0x01 << 0);
}
