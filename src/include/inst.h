#ifndef INST_H
#define INST_H
#include "emu.h"

void clear_screen(struct emu *emu);
void jump(struct emu *emu);
void set_reg_VX(struct emu *emu);
void add_reg_VX(struct emu *emu);
void set_idx(struct emu *emu);
void draw(struct emu *emu);
void logic_arithmetic_inst(struct emu *emu);

#endif