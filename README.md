# Why?

I realized i never built anything as low level as an emulator. I realized my computer-like thinking was becoming more abstract than ever (i hate that feeling), so i wanted to "feel the grass" again by touching something that could give me more satisfaction than web-dev. An emulator seemed a good idea, so why not?

# Setup

**NOTE: this was developed on ubuntu 24.04, as long as you have a c compiler it should be fine**

## Requirements

### SDL

Install with:

```sh
sudo apt update
sudo apt install libsdl2-dev
```

Verify the installation with:

```sh
pkg-config --modversion sdl2
```

# Build & run

**Todo** Improve dev experience. For now just run the following command:

```sh
gcc -Wall -Wextra -Wpedantic -std=c17 src/main.c src/chip8.c src/fonts.c src/display.c src/stack.c src/keypad.c src/cpu.c $(pkg-config --cflags --libs sdl2) -o ./out/chip8 && ./out/chip8 ./roms/IBM_Logo.ch8
```
