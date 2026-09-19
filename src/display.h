#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>
#include <SDL2/SDL.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 10

#define DISPLAY_BG_RGBA 0, 0, 0, 255
#define DISPLAY_FG_RGBA 255, 255, 255, 255

typedef struct
{
    uint8_t pixels[DISPLAY_HEIGHT][DISPLAY_WIDTH];
} Display;

void render_display(SDL_Renderer *renderer, const Display *display);

#endif