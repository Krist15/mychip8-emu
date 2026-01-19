#include "include/emu.h"
#include "include/mask.h"

#include <string.h>

static inline void inc_pc(uint16_t *pc_reg) {
    *pc_reg += 2;
}

void clear_screen(struct emu *emu) {
    memset(emu->display, 0, DISPLAY_SIZE);
    inc_pc(&emu->pc_reg);
}

void jump(struct emu *emu) {
    emu->pc_reg = MASKOFF_NNN(emu->pc_reg);
}

void set_reg_VX(struct emu *emu) {
    emu->gp_regs[MASKOFF_X(emu->pc_reg)] = MASKOFF_NN(emu->pc_reg);
    inc_pc(&emu->pc_reg);
}

void add_reg_VX(struct emu *emu) {
    emu->gp_regs[MASKOFF_X(emu->pc_reg)] += MASKOFF_NN(emu->pc_reg);
    inc_pc(&emu->pc_reg);
}

void set_idx(struct emu *emu) {
    emu->idx_reg = MASKOFF_NNN(emu->pc_reg);
    inc_pc(&emu->pc_reg);
}