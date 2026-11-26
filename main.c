#include "cmd/cmd.h"
#include "hw/hw_resets.h"
#include "hw/hw_clocks.h"
#include "hw/hw_gpio.h"
#include "hw/hw_uart.h"
#include "hw/hw_irq.h"


__attribute__((section(".text.main")))
void setup() {
    // bring IO_BANK0 out of reset
    // wait until IO_BANK0 is out of reset
    hw_resets_out_of_reset(0x01 << 5);
    // init the on-board led: SIO func & output
    hw_gpio_led_init();

    // setup peri clock for the UART
    hw_clocks_clkperi_from_xosc();
    // UART OUT OF RESET
    hw_resets_out_of_reset(0x01 << 22);
    // SETUP UART - hardcoded settings
    // tx rx bufers off - trigger irs on every rx byte
    hw_uart0_init();
    // setup gpio pins 0 & 1 for UART0 tx rx
    hw_gpio_uart0_init();
    // enable the isr for uart 0 in nvic
    hw_interrupts_uart0_enable();
}



__attribute__((section(".text.main")))
void main(void) {
    setup();
    while (1) {
        if (command_ready == 1) {
            command_ready = 0;
            parse_command();
        }
    }
    while(1);
}
