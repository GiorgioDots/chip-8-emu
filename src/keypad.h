#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

uint8_t map_key(SDL_Scancode key);

typedef struct
{
    bool pressed[16];
    bool is_key_pressed;
    uint8_t key_pressed;
} Keypad;

#endif