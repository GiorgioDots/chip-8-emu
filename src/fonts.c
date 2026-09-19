#include "fonts.h"

uint8_t map_font_to_idx(uint8_t font)
{
    return font * FONT_SIZE;
}
