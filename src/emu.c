#include "include/emu.h"
#include "include/inst.h"

void (*op_table[])(struct emu *)   = {
    [0x0000] = clear_screen,
    jump,
    set_reg_VX,
    add_reg_VX,
    set_idx,

};
