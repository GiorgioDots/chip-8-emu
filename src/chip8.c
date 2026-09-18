#include <string.h>
#include <stdio.h>
#include "chip8.h"
#include "fonts.h"

Chip8 initialize_chip8(const uint8_t rom_buffer[], size_t rom_len)
{
    Chip8 chip8 = {0};

    memcpy(&chip8.memory[ROM_PROGRAM_START_IDX], rom_buffer, rom_len);
    memcpy(&chip8.memory[FONT_START_IDX], FONTS, sizeof FONTS);

    return chip8;
}

void tick_delay_timer(Chip8 *chip8)
{
    if (chip8->delay_timer > 0)
    {
        chip8->delay_timer--;
    }
}

void tick_sound_timer(Chip8 *chip8)
{
    if (chip8->sound_timer > 0)
    {
        chip8->sound_timer--;
    }
}

void execute_instruction(Chip8 *chip8)
{
}
