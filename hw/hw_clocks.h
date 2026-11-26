#ifndef HW_CLOCKS_H
#define HW_CLOCKS_H

#include "globals.h"

// ============
//  CLOCKS
// ============
#define CLK_BASE                    0x40008000
#define CLK_REF_CTRL                (CLK_BASE + 0x30)
#define CLK_REF_DIV                 (CLK_BASE + 0x34)
#define CLK_REF_SELECTED            (CLK_BASE + 0x38)
#define CLK_SYS_CTRL                (CLK_BASE + 0x3c)
#define CLK_SYS_SELECTED            (CLK_BASE + 0x44)
#define CLK_PERI_CTRL               0x40008048

// ==========
//  XOSC
// ==========
#define XOSC_BASE                   0x40024000
#define XOSC_CTRL                   (XOSC_BASE + 0x00)
#define XOSC_STATUS                 (XOSC_BASE + 0x04)
#define XOSC_STARTUP                (XOSC_BASE + 0x0c)
#define XOSC_ENABLE_PW              0x00fab000

// ===========
//  PLL
// ===========
#define PLL_BASE                    0x40028000
#define PLL_CS                      (PLL_BASE + 0x00)
#define PLL_PWR                     (PLL_BASE + 0x04)
#define PLL_FBDIV_INT               (PLL_BASE + 0x08)
#define PLL_PRIM                    (PLL_BASE + 0x0c)

// ==========
//  ROSC
// ==========
#define ROSC_BASE                   0x40060000
#define ROSC_DISABLE_PW             0x00d1e000
#define ROSC_CTRL                   (ROSC_BASE + 0x00)


// ==========
//  XOSC
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_xosc_init(void);
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_clkref_from_xosc(uint32 clk_div);

// ==========
//  PLL
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_pll_init(uint32 fb_div, uint32 post_div1, uint32 post_div2, uint32 ref_div);
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_clksys_from_pll();

// ==========
//  ROSC
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_rosc_shutdown();

// ==========
//  CLK_PERI
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_clkperi_from_xosc();

#endif
