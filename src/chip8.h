#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include "display.h"
#include "stack.h"

#define ROM_PROGRAM_START_IDX 0x200
#define INSTRUCTIONS_PER_SECOND 700.0
#define CPU_PERIOD 1.0 / INSTRUCTIONS_PER_SECOND
#define TIMER_PERIOD 1.0 / 60.0
#define FRAME_PERIOD 1.0 / 60.0

typedef struct Chip8
{
    uint8_t memory[4096];
    Display display;
    uint16_t pc; // used only 12 bits
    uint16_t i;  // used only 12 bits
    Stack stack[30];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t v[16]; // v[0xF] also is the flag
} Chip8;

Chip8 initialize_chip8(const uint8_t rom_buffer[], size_t rom_len);
void tick_delay_timer(Chip8 *chip8);
void tick_sound_timer(Chip8 *chip8);
void execute_instruction(Chip8 *chip8);

#endif