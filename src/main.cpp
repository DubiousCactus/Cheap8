/*
 * main.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Chip8.h"
#include <cstdio>

int
main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Usage: %s <chip8_game_image>\n\n", argv[0]);
        return 0;
    }

    printf("Loading: %s ...\n", argv[1]);
    Chip8 cheap8 = Chip8();
    cheap8.Load(argv[1]);
    cheap8.Run();

    return 0;
}
