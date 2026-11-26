#ifndef HW_RESETS_H
#define HW_RESETS_H

#include "globals.h"

// ============
//  RESETS
// ============

#define RESETS_RESET            0x4000c000
#define RESETS_RESET_DONE       0x4000c008

__attribute__((section(".text.hw.hw_resets")))
void hw_resets_out_of_reset(uint32 resets_index_bitmask);

#endif
