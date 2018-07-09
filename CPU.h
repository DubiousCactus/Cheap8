/*
 * CPU.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include "Memory.h"
#include "Stack.h"

#define SCREEN_W 64
#define SCREEN_H 32
#define GAME_OFFSET 0x200

class CPU {

	private:
		/* V0-VF (4 bits index), 8-bit size each */
		uint8_t V[16]; //Registers
		uint16_t I; //Address register, 16 bits wide
		uint16_t PC; //Program counter, 16 bits
		uint16_t opcode; //2 bytes
		uint8_t screen[SCREEN_W * SCREEN_H];
		Memory *ram; //TODO: use smart pointers
		Stack *stack; //Must be at least of size 16
		/* Add the delay timer and the sound timer */

		void execute();


	public:
		CPU();
		~CPU();
		void step();
};

#endif /* !CPU_H */
