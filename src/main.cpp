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

    printf("*****CHEAP 8 EMULATOR*****\n");
    /* TESTING */
    const char* game = "space_invaders.ch8";
    //	if (argc < 2) {
    //	    printf("Usage: %s <chip8_game_image>\n\n", argv[0]);
    //	    return 0;
    //	}
    //
    //	printf("Loading: %s ...\n", argv[1]);

    printf("Loading: %s ...\n", game);
    Chip8 cheap8 = Chip8();
    cheap8.Init();
    //	cheap8.load(argv[1]);
    cheap8.Load(game);
    cheap8.Run();

    return 0;
}
