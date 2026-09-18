#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>

#define ROM_PROGRAM_START_IDX 0x200

typedef struct display_t
{
    uint64_t cols;
    uint32_t rows;
} display_t;

typedef struct Chip8
{
    uint8_t memory[4096];
    display_t display;
    uint16_t pc; // used only 12 bits
    uint16_t i;  // used only 12 bits
    uint16_t stack[30];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t v[16]; // v[0xF] also is the flag
} Chip8;

Chip8 initialize_chip8(const uint8_t rom_buffer[], size_t rom_len);

#endif