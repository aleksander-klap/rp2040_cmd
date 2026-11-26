#include "hw/hw_resets.h"

__attribute__((section(".text.hw.hw_resets")))
void hw_resets_out_of_reset(uint32 resets_index_bitmask){
    // datasheet 2.14.3
    REG32_CLR(RESETS_RESET) = resets_index_bitmask;
    // wait until reset done
    while (!(REG32(RESETS_RESET_DONE) & resets_index_bitmask));
}
