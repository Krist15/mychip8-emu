#include "include/emu.h"
#include "include/mask.h"

#include <string.h>
#include <stdint.h>

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

void add_reg_VX(struct emu *emu) {
    emu->gp_regs[MASKOFF_X(emu->opcode)] += MASKOFF_NN(emu->opcode);
    inc_pc(&emu->pc);
}

void set_idx(struct emu *emu) {
    emu->idx_reg = MASKOFF_NNN(emu->opcode);
    inc_pc(&emu->pc);
}

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