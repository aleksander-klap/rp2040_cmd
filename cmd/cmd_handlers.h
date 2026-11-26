#ifndef CMD_HANDLERS_H
#define CMD_HANDLERS_H

#include "globals.h"
#include "hw/hw_gpio.h"

typedef void (*cmd_handler_t)(uint8 argc, char *argv[]);

typedef struct {
    const char *name;
    cmd_handler_t handler;
} command_t;

__attribute__((section(".text.cmd.cmd_handlers")))
void cmd_led_handler(uint8 argc, char *argv[]);

__attribute__((section(".text.cmd.cmd_handlers")))
void cmd_read_handler(uint8 argc, char *argv[]);

extern const uint8 num_commands;
extern const command_t commands[];

#endif
