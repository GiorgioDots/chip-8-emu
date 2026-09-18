#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

typedef struct bitset12
{
    unsigned value : 12;
} bitset12;

typedef struct display_t
{
    uint64_t cols;
    uint32_t rows;
} display_t;

typedef struct Chip8
{
    uint8_t memory[4096];
    display_t display;
    bitset12 pc;
    uint16_t i;
    uint16_t stack[30];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t registers[16];
} Chip8;

Chip8 *initialize_chip8(void);

#endif