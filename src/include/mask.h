#ifndef MASK_H
#define MASK_H

#define MASKOFF_HB(b) (((b) >> 12) & 0xF)
#define MASKOFF_X(b)  (((b) >> 8) & 0xF)
#define MASKOFF_Y(b)  (((b) >> 4) & 0xF)
#define MASKOFF_N(b)  ((b) & 0xF)
#define MASKOFF_NN(b) ((b) & 0xFF)
#define MASKOFF_NNN(b) ((b) & 0x0FFF)

#endif