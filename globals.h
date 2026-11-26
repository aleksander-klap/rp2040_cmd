#ifndef GLOBALS_H
#define GLOBALS_H

typedef unsigned char           uint8;
typedef unsigned short          uint16;
typedef unsigned int            uint32;

// memory mapped 32-bit register
#define REG32(addr)             (*(volatile uint32 *)(addr))

// atomic register access aliases (datasheet 2.1.2)
// NOT available on SIO (0xd0000000) and on Cortex-M0+ PPB (0xe0000000)
#define REG_ALIAS_XOR           0x1000u
#define REG_ALIAS_SET           0x2000u
#define REG_ALIAS_CLR           0x3000u

#define REG32_XOR(addr)         REG32((addr) + REG_ALIAS_XOR)
#define REG32_SET(addr)         REG32((addr) + REG_ALIAS_SET)
#define REG32_CLR(addr)         REG32((addr) + REG_ALIAS_CLR)

#endif
