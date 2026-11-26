#ifndef HW_IRQ_H
#define HW_IRQ_H

#include "globals.h"

//NVIC
#define NVIC_ISER                  0xe000e100
#define NVIC_ICPR                  0xe000e280

__attribute__((section(".text.hw.hw_irq")))
uint32 hw_interrupts_save_disable_interupts();

__attribute__((section(".text.hw.hw_irq")))
void hw_interrupts_restore_enable_interupts(uint32 primask);

__attribute__((section(".text.hw.hw_irq")))
void hw_interrupts_uart0_enable();

#endif
