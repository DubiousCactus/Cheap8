/*
 * main.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Chip8.h"
#include "Screen.h"

#include <cstdio>
#include <csignal>

Screen* screen;

void
ResizeHandler(int sig)
{
    screen->Resize();
}

int
main(int argc, char** argv)
{
	signal(SIGWINCH, ResizeHandler);
    if (argc < 2) {
        printf("Usage: %s <chip8_game_image>\n\n", argv[0]);
        return 0;
    }

    printf("Loading: %s ...\n", argv[1]);
    screen = new Screen();
    Chip8 cheap8 = Chip8(screen);
    cheap8.Load(argv[1]);
    cheap8.Run();

    //delete screen;

    return 0;
}
