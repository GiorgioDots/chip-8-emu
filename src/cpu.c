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

void sub(Chip8 *chip8, uint8_t a, uint8_t b)
{
    chip8->v[V_FLAG_IDX] = 0;
    if (chip8->v[a] >= chip8->v[b])
    {
        chip8->v[V_FLAG_IDX] = 1;
    }
    chip8->v[a] = chip8->v[a] - chip8->v[b];
}

void shift(Chip8 *chip8, uint8_t x, uint8_t y, bool is_right)
{
    if (false)
    {
        chip8->v[x] = chip8->v[y];
    }

    uint8_t shifted;
    if (is_right)
    {
        shifted = chip8->v[x] & 0x1;
        chip8->v[x] = chip8->v[x] >> 1;
    }
    else
    {
        shifted = chip8->v[x] & 0x80;
        chip8->v[x] = chip8->v[x] << 1;
    }
    chip8->v[V_FLAG_IDX] = shifted;
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
        if (*instruction == 0x00EE)
        {
            // return from subroutine
            uint16_t pop_pc;
            if (pop(chip8->stack, &pop_pc))
            {
                chip8->pc = pop_pc;
            }
        }

        break;

    case 0x1:
        jump(chip8, nnn);
        break;

    case 0x2:
        push(chip8->stack, chip8->pc);
        break;

    case 0x3:
        // skip if equal
        if (chip8->v[x] == nn)
        {
            chip8->pc += 2;
        }
        break;

    case 0x4:
        // skip if not equal
        if (chip8->v[x] != nn)
        {
            chip8->pc += 2;
        }
        break;

    case 0x5:
        // skip if vx == vy
        if (chip8->v[x] == chip8->v[y])
        {
            chip8->pc += 2;
        }
        break;

    case 0x6:
        set_reg_value(chip8, x, nn);
        break;

    case 0x7:
        // carry flag not affected in this instruction
        set_reg_value(chip8, x, chip8->v[x] + nn);
        break;

    case 0x8:
        switch (n)
        {
        case 0x0:
            chip8->v[x] = chip8->v[y];
            break;
        case 0x1:
            chip8->v[x] = chip8->v[x] | chip8->v[y];
            break;
        case 0x2:
            chip8->v[x] = chip8->v[x] & chip8->v[y];
            break;
        case 0x3:
            chip8->v[x] = chip8->v[x] ^ chip8->v[y];
            break;
        case 0x4:
            uint16_t sum = chip8->v[x] + chip8->v[y];
            if (sum > UINT8_MAX)
            {
                chip8->v[V_FLAG_IDX] = 1;
            }
            else
            {
                chip8->v[V_FLAG_IDX] = 0;
            }
            chip8->v[x] = (uint8_t)sum;
            break;
        case 0x5:
            sub(chip8, x, y);
            break;
        case 0x7:
            sub(chip8, y, x);
            break;
        case 0x6:
            shift(chip8, x, y, true);
            break;
        case 0xE:
            shift(chip8, x, y, false);
            break;
        }
        break;

    case 0x9:
        // skip if vx != vy
        if (chip8->v[x] != chip8->v[y])
        {
            chip8->pc += 2;
        }
        break;

    case 0xA:
        chip8->i = nnn;
        break;

    case 0xB:
        uint8_t addr = nnn;
        // superchip
        if (false)
        {
            addr = nnn + chip8->v[x];
        }
        chip8->pc = addr;
        break;

    case 0xC:
        chip8->v[x] = (uint8_t)rand() & nn;
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
