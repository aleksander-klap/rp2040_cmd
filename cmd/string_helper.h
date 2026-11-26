#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include "globals.h"

__attribute__((section(".text.cmd.string_helper")))
uint8 string_eq(const char *a, const char *b);

__attribute__((section(".text.cmd.string_helper")))
uint8 strlen(const char *a);

__attribute__((section(".text.cmd.string_helper")))
uint32 str_addr_to_hex(const char *string);

__attribute__((section(".text.cmd.string_helper")))
void uint32_to_str_hex(const uint32 num, char output[]);

__attribute__((section(".text.cmd.string_helper")))
void uint32_to_str_bin(const uint32 num, char output[]);

#endif
