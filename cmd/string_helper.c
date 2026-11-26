#include "cmd/string_helper.h"

__attribute__((section(".text.cmd.string_helper")))
uint8 string_eq(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    } 
    if (*a == *b) return 1;
    else return 0;
}

__attribute__((section(".text.cmd.string_helper")))
uint8 strlen(const char *a) {
    if (*a == '\0') return 0;
    uint8 len = 0;
    while (*a != '\0') {
        a++;
        len++;
    }
    return len;
}

__attribute__((section(".text.cmd.string_helper")))
uint32 str_addr_to_hex(const char *string) {
    // skip the '0x'
    string += 2;   
    uint32 result = 0;
    while (*string != '\0') {
        result = result << 4;
        if (*string >= '0' && *string <= '9') {
            result = result | (*string - '0');
        } else if (*string >= 'a' && *string <= 'f') {
            result = result | (*string - 'a' + 10);
        } else if (*string >= 'A' && *string <= 'F') {
            result = result | (*string - 'A' + 10);
        } else {
            return 0x99999999;
        }
        string++;
    }
    return result;
}

__attribute__((section(".text.cmd.string_helper")))
void uint32_to_str_hex(const uint32 num, char output[]){
    static const char hex_table[] = "0123456789abcdef";
    uint32 temp;
    uint32 mask = 0xf0000000;
    output[0] = '0';
    output[1] = 'x';
    for (uint8 i = 0; i < 8; i++){
        temp = ((num & mask) >> (28 - (i * 4)));
        output[i+2] = hex_table[temp];
        mask = mask >> 4;
    }
    output[10] = '\0';
}

