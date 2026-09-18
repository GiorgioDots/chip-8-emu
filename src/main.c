#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chip8.h"

static int has_extension(const char *path, const char *extension)
{
    const char *dot = strrchr(path, '.');
    const char *slash = strrchr(path, '/');
    const char *backslash = strrchr(path, '\\');

    if (dot == NULL || (slash != NULL && dot < slash) || (backslash != NULL && dot < backslash))
    {
        return 0;
    }

    return strcmp(dot, extension) == 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Please provide a ROM path.\n");
        return 1;
    }

    char *file_path = argv[1];

    if (!has_extension(file_path, ".ch8"))
    {
        fprintf(stderr, "File must have the .ch8 extension.\n");
        return 1;
    }

    FILE *rom = fopen(file_path, "rb");
    if (rom == NULL)
    {
        fprintf(stderr, "File does not exist or cannot be opened: %s\n", file_path);
        return 1;
    }

    fseek(rom, 0, SEEK_END);
    long rom_len = ftell(rom);
    rewind(rom);

    uint8_t rom_buffer[rom_len];

    fread(rom_buffer, 1, rom_len, rom);
    fclose(rom);

    printf("Read file %s, size %ld\n", file_path, sizeof rom_buffer);

    Chip8 chip8 = initialize_chip8(rom_buffer, rom_len);
    return 0;
}