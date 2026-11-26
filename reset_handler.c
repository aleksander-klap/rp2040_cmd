#include "globals.h"
#include "hw/hw_resets.h"
#include "hw/hw_clocks.h"

extern void main();

extern uint8 _start_data[];
extern uint8 _end_data[];
extern uint8 _start_data_flash[];
__attribute__((section(".text.reset_handler")))
void sw_copy_data_section() {
    uint8 *dst = _start_data;
    uint8 *src = _start_data_flash;

    while (dst < _end_data) {
        *dst++ = *src++;
    }
}
extern uint8 _start_bss[];
extern uint8 _end_bss[];
__attribute__((section(".text.reset_handler")))
void sw_zero_bss_section() {
    for (uint8 *p = _start_bss; p < _end_bss; p++)
        *p = 0;
}
__attribute__((section(".text.reset_handler")))
void reset_handler() {
    hw_clocks_xosc_init();
    hw_clocks_clkref_from_xosc(0x01);
    hw_resets_out_of_reset(0x01 << 12);
    hw_clocks_pll_init(0x7d, 0x06, 0x02, 0x01);
    hw_clocks_clksys_from_pll();
    hw_clocks_rosc_shutdown();
    sw_copy_data_section();
    sw_zero_bss_section();

    main();
    // unreachable
    while(1);
}
