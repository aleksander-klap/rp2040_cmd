#include "hw/hw_irq.h"

__attribute__((section(".text.hw.hw_irq")))
uint32 hw_interrupts_save_disable_interupts(){
    // save the pending interrupts mask - than disable interrupts
    uint32 primask;
    __asm__ volatile ("mrs %0, PRIMASK \n\t" "cpsid i" : "=r" (primask) : : "memory");
    return primask;
}

__attribute__((section(".text.hw.hw_irq")))
void hw_interrupts_restore_enable_interupts(uint32 primask){
    // restore the irq pending interrupts mask - then enabl;e interrupts
    __asm__ volatile ("msr PRIMASK, %0" : : "r" (primask) : "memory" );
}

__attribute__((section(".text.hw.hw_irq")))
void hw_interrupts_uart0_enable() {
    // setup nvic mask
    // disable interrupts - preserve pending
    uint32 primask = hw_interrupts_save_disable_interupts();
    // clear pending interrupts on UART_0
    REG32(NVIC_ICPR) = (0x01 << 20);
    // enable uart0 interrupt
    REG32(NVIC_ISER) = (0x01 << 20);
    // enable and restore intertupts
    hw_interrupts_restore_enable_interupts(primask);
}
