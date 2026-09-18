#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32
#define DISPLAY_SCALE 10

typedef struct
{
    uint8_t pixels[DISPLAY_HEIGHT][DISPLAY_WIDTH];
} Display;

#endif