#include "display.h"
#include "chip8.h"

void render_display(SDL_Renderer *renderer, const Display *display)
{
    SDL_SetRenderDrawColor(renderer, DISPLAY_BG_RGBA);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, DISPLAY_FG_RGBA);

    // Render display
    for (uint8_t y = 0; y < DISPLAY_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < DISPLAY_WIDTH; x++)
        {
            if (display->pixels[y][x])
            {
                SDL_Rect pixel = {
                    .x = x * DISPLAY_SCALE,
                    .y = y * DISPLAY_SCALE,
                    .w = DISPLAY_SCALE,
                    .h = DISPLAY_SCALE};

                SDL_RenderFillRect(renderer, &pixel);
            }
        }
    }

    SDL_RenderPresent(renderer);
}
