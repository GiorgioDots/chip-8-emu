#include <stdint.h>
#include <stdbool.h>
#include "cpu.h"
#include "fonts.h"

#pragma region Instructions Implementations
void clear_screen(Chip8 *chip8)
{
    for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
        {
            chip8->display.pixels[y][x] = 0;
        }
    }
}

void jump(Chip8 *chip8, uint16_t nnn)
{
    chip8->pc = nnn;
}

void set_reg_value(Chip8 *chip8, uint8_t reg, uint8_t n)
{
    chip8->v[reg] = n;
}

void display(Chip8 *chip8, uint8_t n_pixels_h, uint8_t vx_reg, uint8_t vy_reg)
{
    uint8_t x = chip8->v[vx_reg] % DISPLAY_WIDTH;
    uint8_t y = chip8->v[vy_reg] % DISPLAY_HEIGHT;

    chip8->v[V_FLAG_IDX] = 0;

    for (uint8_t n = 0; n < n_pixels_h; n++)
    {
        uint8_t row_x = x;
        uint8_t font_byte = chip8->memory[chip8->i + n];
        uint8_t font_mask_start = 0x80;
        for (uint8_t pi = 0; pi < 8; pi++)
        {
            // Starting from most significant bit (10000000 >> 0)
            uint8_t font_bit = font_byte & (font_mask_start >> pi);

            // If both px are on, set flag to 1
            if (chip8->display.pixels[y][row_x] & font_bit)
            {
                chip8->v[V_FLAG_IDX] = 1;
            }

            chip8->display.pixels[y][row_x] ^= font_bit;

            row_x++;

            if (row_x >= DISPLAY_WIDTH)
            {
                break;
            }
        }

        y++;

        if (y >= DISPLAY_HEIGHT)
        {
            break;
        }
    }
}

#pragma endregion

bool cpu_fetch(Chip8 *chip8, uint16_t *instruction)
{
    if (chip8->pc + 1 >= CHIP8_MEMORY_SIZE)
    {
        return false;
    }

    *instruction = ((uint16_t)chip8->memory[chip8->pc] << 8) | chip8->memory[chip8->pc + 1];
    // printf("Fetched instruction %04x at %x\n", *instruction, chip8->pc);
    chip8->pc += 2;
    return true;
}

void cpu_decode_execute(Chip8 *chip8, uint16_t *instruction)
{
    // First nimble = opcode
    uint16_t opcode = (*instruction & 0xF000) >> 12;
    uint8_t x = (*instruction & 0x0F00) >> 8;
    uint8_t y = (*instruction & 0x00F0) >> 4;
    uint8_t n = *instruction & 0x000F;
    uint8_t nn = *instruction & 0x00FF;
    uint16_t nnn = *instruction & 0x0FFF;

    // printf("Opcode %1x, nnn %3x, from %4x\n", opcode, nnn, *instruction);

    switch (opcode)
    {
    case 0x0:
        if (*instruction == 0x00E0)
        {
            clear_screen(chip8);
        }
        break;

    case 0x1:
        jump(chip8, nnn);
        break;

    case 0x6:
        set_reg_value(chip8, x, nn);
        break;

    case 0x7:
        // carry flag not affected in this instruction
        set_reg_value(chip8, x, chip8->v[x] + nn);
        break;

    case 0xA:
        chip8->i = nnn;
        break;

    case 0xD:
        display(chip8, n, x, y);
        break;

    default:
        break;
    }
}

void cpu_loop(Chip8 *chip8)
{
    uint16_t instruction;
    if (!cpu_fetch(chip8, &instruction))
    {
        return;
    }
    cpu_decode_execute(chip8, &instruction);
}
