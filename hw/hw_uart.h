#ifndef HW_UART_H
#define HW_UART_H

//UART
#define UART0_DR                   0x40034000
#define UART0_TFR                  0x40034018
#define UART0_IBDR                 0x40034024
#define UART0_FBDR                 0x40034028
#define UART0_LCR_H                0x4003402c
#define UART0_CR                   0x40034030
#define UART0_IMSC                 0x40034038
#define UART0_ICR                  0x40034044

__attribute__((section(".text.hw.hw_uart")))
void hw_uart0_init();

#endif
