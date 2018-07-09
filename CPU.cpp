/*
 * CPU.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include <cstring>
#include "CPU.h"


CPU::CPU() {
	stack = new Stack();
	ram = Memory::getInstance();
	memset(V, 0, 16);
	memset(screen, 0, SCREEN_H * SCREEN_W);
	I = 0;
	PC = GAME_OFFSET;
	opcode = 0;
}

CPU::~CPU() {
	delete stack;
	delete ram;
}

void CPU::step() {
	printf("[*] CPU step (PC=%02X): fetching opcode...\n", PC);
	opcode = ram->readOpCode(PC); //Fetch next opcode in the RAM
	execute();
	PC += 2; //An opcode is 2 bytes while the RAM is byte encoded
}

void CPU::execute() {

	printf("[*] Executing opcode: %02X\n", opcode);

	//TODO: Use sub-switches because (A) == X won't work!
	switch (opcode) {
		//case opcode & 0xF000: //0NNN
		case 0x00E0: //Clear the screen
			clearScreen();
			break;
		case 0x00EE: //Returns from subroutine
			return;
			break;
		case (opcode & 0xF000) == 0x1000: //Jump to address
			uint16_t address = opcode & 0x0FFF;
			//TODO: set address register ? Set program counter ?
			break;
		case (opcode & 0xF000) == 0x2000: //Call subroutine
			uint16_t address = opcode & 0x0FFF;
			//TODO: set program counter ?...
			break;
		case (opcode & 0xF000) == 0x3000: //Skip next opcode if VX = NN
			short register_index = opcode & 0x0F00;
			uint8_t value = opcode & 0x00FF;
			if (register_index < 0 || register_index >= 16) {
				std::cout << "ERROR: invalid register";
				exit(1);
			}

			if (V[register_index] == value)
				PC += 2; //TODO: check that !
			break;
		case (opcode & 0xF000) == 0x4000: //Skip next opcode if VX != NN
			short register_index = opcode & 0x0F00;
			uint8_t value = opcode & 0x00FF;
			if (register_index < 0 || register_index >= 16) {
				std::cout << "ERROR: invalid register";
				exit(1);
			}

			if (V[register_index] != value)
				PC += 2; //TODO: check that !
			break;
		case (opcode & 0xF00F) == 0x5000: //Skip next opcode if VX == VY
			short r1 = opcode & 0x0F00,
				  r2 = opcode & 0x00F0;
			if (r1 < 0 || r1 >= 16 || r2 < 0 || r2 >= 16) {
				std::cout << "ERROR: invalid register";
				exit(1);
			}
			if (V[r1] == V[r2])
				PC += 2; //TODO: check that !
			break;
		case (opcode & 0xF000) == 0x6000: //Set VX = NN
			short register_index = opcode & 0x0F00;
			uint8_t value = opcode & 0x00FF;
			if (register_index < 0 || register_index >= 16) {
				std::cout << "ERROR: invalid register";
				exit(1);
			}
			V[register_index] = value;
			break;
		case (opcode & 0xF000) == 0x7000: //Set VX += NN (no carry flag)
			short register_index = opcode & 0x0F00;
			uint8_t value = opcode & 0x00FF;
			if (register_index < 0 || register_index >= 16) {
				std::cout << "ERROR: invalid register";
				exit(1);
			}
			V[register_index] += value;
			break;
		default:
	}
}
