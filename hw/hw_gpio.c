#include "hw/hw_gpio.h"
#include "globals.h"

__attribute__((section(".text.hw.hw_gpio")))
void hw_gpio_led_init(void) {
    // just a helper func for the onboard LED
    // SET gpio25 to func5 - SIO
    REG32(GPIO25_CTRL) = 0x05;
    // SET gpio25 to output
    REG32(SIO_GPIO_OE_ASET) = (0x01 << 25);
}

__attribute__((section(".text.hw.hw_gpio")))
void hw_gpio_uart0_init(void) {
    // setsup UART0 pins into the right uart functions (rx & tx)
    // SET gpio0 to func2 (uart tx)
    REG32(GPIO0_CTRL) = 0x02;
    // SET gpio1 to func2 (uart rx)
    REG32(GPIO1_CTRL) = 0x02;
}
