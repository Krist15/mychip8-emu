#ifndef MASK_H
#define MASK_H

#define MASKOFF_HB(b) ((b >> 12) & 0xF)
#define MASKOFF_X(b) ((b >> 8) & 0xF)
#define MASKOFF_Y(b) ((b >> 4) & 0xF)
#define MASKOFF_N(b) (b & 0xF)
#define MASKOFF_NN(b) (b & (MASKOFF_Y(b) | MASKOFF_N(b)))
#define MASKOFF_NNN(b) ((b & (MASKOFF_X(b) | MASKOFF_Y(b) | MASKOFF_N(b))))

#endif