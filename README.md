# Build & run

`gcc -Wall -Wextra -Wpedantic -std=c17 src/main.c src/chip8.c src/fonts.c src/display.c src/stack.c $(pkg-config --cflags --libs sdl2) -o ./out/chip8 && ./out/chip8 ./roms/IBM_Logo.ch8`
