#ifndef HW_GPIO_H
#define HW_GPIO_H

//IO_BANK0 GPIO
#define GPIO0_CTRL                 0x40014004
#define GPIO1_CTRL                 0x4001400c
#define GPIO25_CTRL                0x400140cc

//SIO GPIO
// SIO has its own SET / CLR / XOR registers - REG32_SET/CLR/XOR do NOT work on SIO (datasheet 2.1.2)
#define SIO_GPIO_OUT_ASET          0xd0000014
#define SIO_GPIO_OUT_ACLR          0xd0000018
#define SIO_GPIO_OUT_XOR           0xd000001c
#define SIO_GPIO_OE_ASET           0xd0000024

__attribute__((section(".text.hw.hw_gpio")))
void hw_gpio_led_init();

__attribute__((section(".text.hw.hw_gpio")))
void hw_gpio_uart0_init();

#endif
