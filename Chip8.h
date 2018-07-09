/*
 * Chip8.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include "CPU.h"
#include "Memory.h"

class Chip8 {

	private:
		bool running;
		uint8_t opcode;
		uint8_t key[16]; //Hex based keypad
		CPU *cpu;
		Memory *ram;

	public:
		Chip8();
		~Chip8();
		void init();
		void cycle();
		void drawGraphics();
		void setKeys();
		void mainLoop();
		void run();
		void stop();
		void load(const char *file_name);

};

#endif /* !CHIP8_H */
