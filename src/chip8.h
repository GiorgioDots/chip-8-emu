#ifndef CHIP8_H
#define CHIP8_H

#include <stdint.h>
#include "display.h"
#include "stack.h"
#include "keypad.h"

#define CHIP8_MEMORY_SIZE 4096
#define ROM_PROGRAM_START_IDX 0x200
#define INSTRUCTIONS_PER_SECOND 700.0
#define CPU_PERIOD 1.0 / INSTRUCTIONS_PER_SECOND
#define TIMER_PERIOD 1.0 / 60.0
#define FRAME_PERIOD 1.0 / 60.0
#define V_FLAG_IDX 0xF

typedef struct Chip8
{
    uint8_t memory[CHIP8_MEMORY_SIZE];
    Display display;
    uint16_t pc; // used only 12 bits
    uint16_t i;  // used only 12 bits
    Stack stack[30];
    uint8_t delay_timer;
    uint8_t sound_timer;
    uint8_t v[16]; // v[0xF] also is the flag
    Keypad keypad;
} Chip8;

Chip8 chip8_initialize(const uint8_t rom_buffer[], size_t rom_len);
void chip8_tick_delay_timer(Chip8 *chip8);
void chip8_tick_sound_timer(Chip8 *chip8);
void chip8_loop(Chip8 *chip8);

#endif