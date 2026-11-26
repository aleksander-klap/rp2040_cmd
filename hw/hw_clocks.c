#include "hw/hw_clocks.h"

// ==========
//  XOSC
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_xosc_init(void){
    // setup frequency range 1-15MHZ
    REG32(XOSC_CTRL) = 0xaa0;
    // setup startup delay
    REG32(XOSC_STARTUP) = 0x2f;
    // enable xosc with password
    REG32_SET(XOSC_CTRL) = XOSC_ENABLE_PW;
    // wait until its status bit is 1
    while (!(REG32(XOSC_STATUS) & (0x01 << 31)));
}

__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_clkref_from_xosc(uint32 clk_div) {
    // set clock divider for clk_ref
    REG32(CLK_REF_DIV) = (clk_div << 8);
    // clk_ref_ctrl - set 0x2 - sets xosc as source for clk_ref
    REG32_SET(CLK_REF_CTRL) = 0x02;
    // wait until right source is selected
    while (!(REG32(CLK_REF_SELECTED) & (0x01 << 2)));
}

// ==========
//  PLL
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_pll_init(uint32 fb_div, uint32 post_div1, uint32 post_div2, uint32 ref_div) {
    // setup PLL clock divisor
    REG32(PLL_CS) = (ref_div << 0);
    //setup feedback divisor
    REG32(PLL_FBDIV_INT) = fb_div;
    // turn the power on - ppl and ppl vco
    REG32_CLR(PLL_PWR) = 0x21;
    // wait until locked
    while (!(REG32(PLL_CS) & (0x01 << 31)));
    //set post divisors
    REG32(PLL_PRIM) = ((post_div1 << 16) | (post_div2 << 12));
    // power on postdivider
    REG32_CLR(PLL_PWR) = (0x01 << 3);
}

__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_clksys_from_pll(){
    // datasheet 2.15.7 - select sys clock from aux source - pll
    REG32(CLK_SYS_CTRL) = 0x01;
    // wait till stable 
    while (!(REG32(CLK_SYS_SELECTED) & (0x01 << 1)));
}

// ==========
//  ROSC
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_rosc_shutdown(){
    REG32(ROSC_CTRL) = ROSC_DISABLE_PW;
}

// ==========
//  CLK_PERI
// ==========
__attribute__((section(".text.hw.hw_clocks")))
void hw_clocks_clkperi_from_xosc(){
    // setup clk_peri
    // enable + 0x04 = xosc as source
    REG32(CLK_PERI_CTRL) = (0x01 << 11) | (0x04 << 5);
}
