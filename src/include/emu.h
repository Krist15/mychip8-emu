#ifndef EMU_H
#define EMU_H

#include <stdint.h>

#define MEMORY_SIZE 4096
#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SIZE DISPLAY_WIDTH * DISPLAY_HEIGHT
#define STACK_SIZE 16


struct emu {
    uint8_t memory[MEMORY_SIZE];
    uint8_t display[DISPLAY_SIZE];
    uint16_t stack[STACK_SIZE];
    uint8_t gp_regs[16];
    uint16_t idx_reg;
    uint8_t pc_reg;
};

extern void (*op_table[])(struct emu*);

#endif