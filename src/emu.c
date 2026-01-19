#include "include/emu.h"
#include "include/inst.h"
#include "include/mask.h"
#include <stdint.h>
#include <string.h>

void (*op_table[])(struct emu *) = {
    [0x0] = clear_screen,
    [0x1] = jump,
    [0x6] = set_reg_VX,
    [0x7] = add_reg_VX,
    [0xA] = set_idx,
    [0xD] = draw,
};

void read_rom(FILE *rom, struct emu *emu) {
    memset(emu, 0, sizeof(*emu));
    fread(emu->memory + 0x200, sizeof(uint8_t), MEMORY_SIZE - 0x200, rom);
    emu->pc = 0x200;
    emu->opcode = 0;
}

void execute(struct emu *emu) {
    emu->opcode = (emu->memory[emu->pc] << 8) | emu->memory[emu->pc + 1];
    uint8_t opcode_hi = MASKOFF_HB(emu->opcode);
    if (op_table[opcode_hi])
        op_table[opcode_hi](emu);
    else
        emu->pc += 2;
}