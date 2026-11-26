#include "cmd/cmd_handlers.h"
#include "cmd/uart_print.h"
#include "cmd/string_helper.h"

__attribute__((section(".text.cmd.cmd_handlers")))
void cmd_led_handler(uint8 argc, char *argv[]){
    if (argc != 2) {
        uart_puts("Usage: led < on / off / toggle > ", 1); 
        return;
    }
    if (string_eq(argv[1], "on")) {
        REG32(SIO_GPIO_OUT_ASET) = (0x01 << 25); 
        return;
    }
    if (string_eq(argv[1], "off")) {
        REG32(SIO_GPIO_OUT_ACLR) = (0x01 << 25); 
        return;
    }
    if (string_eq(argv[1], "toggle")) {
        REG32(SIO_GPIO_OUT_XOR) = (0x01 << 25); 
        return;
    }
    uart_putnl();
    uart_puts("Usage: led < on / off / toggle > ", 1); 
}

__attribute__((section(".text.cmd.cmd_handlers")))
void cmd_read_handler(uint8 argc, char *argv[]){
    if (argc != 2) {
        uart_puts("Usage: read <hex addr> (eg. read 0x40041023)", 1);
        return;
    }
    if(strlen(argv[1]) != 10){
        uart_puts("strlen wrong", 1);
        uart_puts("Pass valid address as argument (eg. 0x12345678)", 1);
        return;
    }
    if(argv[1][0] != '0' || argv[1][1] != 'x') {
        uart_puts("in check of first 2 chars 0x", 1);
        uart_puts("Pass valid address as argument (eg. 0x12345678)", 1);
        return;
    }
    uint32 addr = str_addr_to_hex(argv[1]);
    if (addr == 0x99999999) {
        uart_puts("in str_addr_to_hex", 1);
        uart_puts("Pass valid address as argument (eg. 0x12345678)", 1);
        return;
    }
    uint32 read_value = *(uint32 *)addr; 
    char output[11];
    uint32_to_str_hex(read_value, output);
    uart_puts("Hex: ", 0);
    uart_puts(output, 1);
}

const uint8 num_commands = 2;
const command_t commands[] = {
    {"led", cmd_led_handler},
    {"read", cmd_read_handler}
};
