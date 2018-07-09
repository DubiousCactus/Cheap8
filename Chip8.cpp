/*
 * Chip8.cpp
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include "Chip8.h"


Chip8::Chip8() {
	running = false;
	cpu = new CPU();
	ram = Memory::getInstance();
}


Chip8::~Chip8() {
	delete cpu;
	//delete ram;
}


void Chip8::init() {
	/* Init registers and memory */

}


void Chip8::cycle() {
	/* Run next instruction */
	printf("[*] Running one cycle\n");
	cpu->step();
	std::cin.get();
	/* Update timers */
}

void Chip8::drawGraphics() {
	//Write the pixels array
}


void Chip8::setKeys() {
	/* meh */
}


void Chip8::mainLoop() {

	/* Run this inside a thread */
	while (this->running) {
		cycle();
		drawGraphics();
		setKeys();
	}
}


void Chip8::run() {
	printf("[*] Running...\n");
	if (!running) {
		running = true;
		/* Start the mainLoop thread */
		mainLoop();
	} else {
		return;
	}
}


void Chip8::stop() {
	running = false;
}

void Chip8::load(const char *file_name) {
	FILE *file = fopen(file_name, "rb");
	if (file != NULL) {
		uint8_t *bytes;
		/* Read file size */
		fseek(file, 0, SEEK_END);
		long file_size = ftell(file);
		rewind(file);

		/* Read the whole file as a block */
		bytes = (uint8_t *) malloc(sizeof(uint8_t) * file_size);
		size_t size_read = fread(bytes, 1, file_size, file);

		if (size_read != file_size) {
			printf("[!] ERROR: could not read %s\n", file_name);
			exit(1);
		}

		if (!ram->writeBytes(0x200, size_read, bytes)) {
			printf("[!] ERROR: could not write the game in RAM\n");
			exit(1);
		}

		fclose(file);
		free(bytes);
	}
}
