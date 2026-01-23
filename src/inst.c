#include "include/inst.h"
#include "include/emu.h"
#include "include/mask.h"

#include <string.h>
#include <stdint.h>

#define SET 0x0
#define BINARY_OR 0x1
#define BINARY_AND 0x2
#define LOGICAL_XOR 0x3
#define BINARY_ADD 0x4

void logic_arithmetic_inst(struct emu *emu) {
    switch (MASKOFF_N(emu->opcode)) {
        case (SET):
            emu->gp_regs[MASKOFF_X(emu->opcode)] = emu->gp_regs[MASKOFF_Y(emu->opcode)];
        case (BINARY_OR):
            // binary OR instruction
            emu->gp_regs[MASKOFF_X(emu->opcode)] |= emu->gp_regs[MASKOFF_Y(emu->opcode)];
        case (BINARY_AND):
            // binary AND instruction
            emu->gp_regs[MASKOFF_X(emu->opcode)] &= emu->gp_regs[MASKOFF_Y(emu->opcode)];
        case (LOGICAL_XOR):
            // logical XOR instruction
            emu->gp_regs[MASKOFF_X(emu->opcode)] ^=  emu->gp_regs[MASKOFF_Y(emu->opcode)];
        case (BINARY_ADD):
            // VX is set to the value of VX plus the value of VY. VY is not affected.
            /* Unlike 7XNN, this addition will affect the carry flag. 
            If the result is larger than 255 (and thus overflows the 8-bit register VX), the flag register VF is set to 1. 
            If it doesn’t overflow, VF is set to 0.*/
            emu->gp_regs[MASKOFF_X(emu->opcode)] +=  emu->gp_regs[MASKOFF_Y(emu->opcode)];
        case (0x5):
        case (0x6):
        case (0x7):
        case (0xE):
        default: 
            printf("Unknown logical/binaryinstruction");
    }
}

static inline void inc_pc(uint16_t *pc_reg) {
    *pc_reg += 2;
}

void clear_screen(struct emu *emu) {
    memset(emu->display, 0, DISPLAY_SIZE);
    inc_pc(&emu->pc);
}

void jump(struct emu *emu) {
    emu->pc = MASKOFF_NNN(emu->opcode);
}

void set_reg_VX(struct emu *emu) {
    emu->gp_regs[MASKOFF_X(emu->opcode)] = MASKOFF_NN(emu->opcode);
    inc_pc(&emu->pc);
}

void set_idx(struct emu *emu) {
    emu->idx_reg = MASKOFF_NNN(emu->opcode);
    inc_pc(&emu->pc);
}

void add_reg_VX(struct emu *emu) {
    emu->gp_regs[MASKOFF_X(emu->opcode)] += MASKOFF_NN(emu->opcode);
    inc_pc(&emu->pc);
}


void call(struct emu *emu) { // 2NNN
    emu->pc = MASKOFF_NNN(emu->opcode);
}

// void set_reg_VX(struct emu *emu) {
//     emu->gp_regs[MASKOFF_X(emu->pc)] = MASKOFF_N(emu->pc);
// }

static void set_VF(struct emu *emu, uint16_t flag) {
    emu->gp_regs[0xF] = flag;
}

static inline uint8_t test_bit(const uint8_t byte, uint8_t idx) {
    return byte & (0x1 << idx);
}

// static inline void set_bit(uint8_t *byte, uint8_t idx) {
//     *byte |= 0x1 << idx;
// }

// static inline void clear_bit(uint8_t *byte, uint8_t idx) {
//     *byte &= ~(0x1 << idx);
// }

void draw(struct emu *emu) {
    uint8_t x = emu->gp_regs[MASKOFF_X(emu->opcode)] % DISPLAY_WIDTH;
    uint8_t y = emu->gp_regs[MASKOFF_Y(emu->opcode)] % DISPLAY_HEIGHT;
    uint8_t height = MASKOFF_N(emu->opcode);
    uint8_t pixel;
    uint8_t collision = 0;

    for (uint8_t row = 0; row < height; row++) {
        pixel = emu->memory[emu->idx_reg + row];
        for (uint8_t col = 0; col < 8; col++) {
            if (test_bit(pixel, 7 - col)) {
                uint16_t display_idx = ((y + row) % DISPLAY_HEIGHT) * DISPLAY_WIDTH + ((x + col) % DISPLAY_WIDTH);
                if (emu->display[display_idx] == 1) {
                    collision = 1;
                }
                emu->display[display_idx] ^= 1;
            }
        }
    }

    set_VF(emu, collision);
    inc_pc(&emu->pc);
}