#include "cmd/cmd.h"
#include "cmd/cmd_handlers.h"
#include "cmd/string_helper.h"
#include "cmd/uart_print.h"

volatile uint8 command_ready = 0;
volatile uint8 buff_ptr = 0;
char uart_buff[UART_BUFF_SIZE];

__attribute__((section(".text.cmd.debug")))
void debug_print_buffer(const char *buff, uint8 size)
{
    uart_puts("[", 0);
    for (uint8 i = 0; i < size; i++) {
        switch (buff[i]) {
        case '\0':
            uart_puts("\\0", 0);
            break;
        case '\r':
            uart_puts("\\r", 0);
            break;
        case '\n':
            uart_puts("\\n", 0);
            break;
        case '\t':
            uart_puts("\\t", 0);
            break;
        case ' ':
            uart_puts("_", 0);       // make spaces visible
            break;
        default:
            if (buff[i] >= 32 && buff[i] <= 126) {
                uart_putc(buff[i]);
            } else {
                uart_putc('.');
            }
            break;
        }
        if (i != size - 1)
            uart_putc('|');
    }
    uart_puts("]", 1);
}

__attribute__((section(".text.cmd.cmd")))
void tokenize(char *buff, uint8 buff_size, char *argv[], uint8 *argc){
    if (buff[0] == '\0') {
        *argc = 0;
        return;
    }
    argv[0] = buff;
    *argc = 1;
    for (uint8 i = 0; buff[i] != '\0'; i++) {
        if (buff[i] == ' ') {
            buff[i] = '\0';
            if (buff[i+1] != '\0' && buff[i+1] != ' ') {
                argv[*argc] = &buff[i + 1];
                (*argc) += 1;
            } 
        }
    }
}

__attribute__((section(".text.cmd.cmd")))
void parse_command() {
    char *argv[ARGV_MAX]; 
    uint8 argc;
    tokenize(uart_buff, UART_BUFF_SIZE, argv, &argc);
    if (argc == 0) {
        uart_puts("Unknown command", 1);
        buff_ptr = 0;
        uart_buff[0] = '\0';
        return;
    }
    for (uint8 i = 0; i < num_commands; i++) {
        if(string_eq(argv[0], commands[i].name)){
            commands[i].handler(argc, argv);
            buff_ptr = 0;
            uart_buff[0] = '\0';
            return;
        }
    }
    uart_puts("Unknown command", 1);
    buff_ptr = 0;
    uart_buff[0] = '\0';
}


