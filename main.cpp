/*
 * main.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include "Chip8.h"

int main(int argc, char **argv) {

	printf("*****CHEAP 8 EMULATOR*****\n");
	/* TESTING */
	const char *game = "space_invaders.ch8";
//	if (argc < 2) {
//	    printf("Usage: %s <chip8_game_image>\n\n", argv[0]);
//	    return 0;
//	}
//
//	printf("Loading: %s ...\n", argv[1]);

	printf("Loading: %s ...\n", game);
	Chip8 cheap8 = Chip8();
	cheap8.init();
//	cheap8.load(argv[1]);
	cheap8.load(game);
	cheap8.run();

	return 0;
}
