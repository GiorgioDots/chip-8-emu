#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "chip8.h"
#include "display.h"

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

    // Init graphics
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL initialization failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        DISPLAY_WIDTH * DISPLAY_SCALE,
        DISPLAY_HEIGHT * DISPLAY_SCALE,
        0);

    if (window == NULL)
    {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL)
    {
        SDL_Log("Renderer creation failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int running = 1;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Render logic

            SDL_RenderPresent(renderer);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}