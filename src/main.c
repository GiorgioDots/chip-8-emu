#include <stdio.h>
#include "chip8.h"

int main(void)
{
    Chip8 *chip8 = initialize_chip8();
    printf("Hello %li\r\n", sizeof *chip8->memory);
    return 0;
}