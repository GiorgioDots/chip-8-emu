#include <string.h>
#include <stdio.h>
#include "chip8.h"
#include "fonts.h"
#include "cpu.h"

Chip8 chip8_initialize(const uint8_t rom_buffer[], size_t rom_len)
{
    Chip8 chip8 = {0};

    chip8.pc = ROM_PROGRAM_START_IDX;
    memcpy(&chip8.memory[ROM_PROGRAM_START_IDX], rom_buffer, rom_len);
    memcpy(&chip8.memory[FONT_START_IDX], FONTS, sizeof FONTS);

    return chip8;
}

void chip8_tick_delay_timer(Chip8 *chip8)
{
    if (chip8->delay_timer > 0)
    {
        chip8->delay_timer--;
    }
}

void chip8_tick_sound_timer(Chip8 *chip8)
{
    if (chip8->sound_timer > 0)
    {
        chip8->sound_timer--;
    }
}

void chip8_loop(Chip8 *chip8)
{
    cpu_loop(chip8);
}
