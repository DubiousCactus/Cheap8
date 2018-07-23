/*
 * CPU.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
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
	JMP = false;
	srand(time(NULL));
}

CPU::~CPU() {
	delete stack;
	delete ram;
}

void CPU::step() {
	printf("[*] CPU step (PC=%02X): fetching opcode...\n", PC);
	opcode = ram->readOpCode(PC); //Fetch next opcode in the RAM
	execute();
	if (!JMP) {
		PC += 2; //An opcode is 2 bytes while the RAM is byte encoded
	} else {
		JMP = false;
	}
	//TODO: Use a timer to slow the CPU down till it's playable (what's the reference ?)
}

void CPU::clearScreen() {
	memset(screen, 0, SCREEN_H * SCREEN_W);
}

void CPU::draw(const uint8_t x, const uint8_t y, const uint8_t height) {
	//TODO
	/* Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value doesn’t change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen 
 	 */
}

uint8_t CPU::getDelay() {
	return delay_timer;
}

uint8_t CPU::getKey() {
	//TODO: returns the pressed key
	return 0;
}

void CPU::setDelay(uint8_t value) {
	delay_timer = value;
}

void CPU::setSoundTimer(uint8_t value) {
	sound_timer = value;
}

uint16_t CPU::getSpriteAddr(uint8_t register) {
	//TODO
	return 0x000;
}

void CPU::dumpRegisters(uint8_t register, uint16_t addr) {
	//TODO
}

void CPU::loadRegisters(uint8_t register, uint16_t addr) {
	//TODO
}


void CPU::setBCD(uint8_t register) {
	/* TODO: Simplify this... Do BCD + binary mask instead of this BS */
	*(I+0) = register / 100;
	*(I+1) = (register / 10) - (register / 100); 
	*(I+2) = register;
}

void CPU::execute() {

	printf("[*] Executing opcode: 0x%02X\n", opcode);

	switch (opcode & 0xF000) {
		case 0x0000:
			switch (opcode & 0x000F) {
				case 0x00E0:
					clearScreen();
					break;
				case 0x00EE:
					return; //Return from subroutine. What does that imply actually ??
					break;
				default:
					break;
			}
			break;
		case 0x1000: //Jump to address
			{
				uint16_t address = opcode & 0x0FFF;
				PC = address;
				JMP = true;
				break;
			}
		case 0x2000: //Call subroutine
			{
				uint16_t address = opcode & 0x0FFF;
				//TODO: set program counter ?... Store return address in the stack ?
				PC = address;
				JMP = true;
				break;
			}
		case 0xA000: //Set I = NNN
			I = (opcode & 0x0FFF);
			break;
		case 0xB000: //Jump to NNN + V0
			PC = (opcode & 0x0FFF) + V[0];
			JMP = true;
			break;
		case 0xC000: //Set VX = NN & random()
			V[opcode & 0x0F00] = rand() & (opcode & 0x00FF); //TODO: Make sure this is done right!
			break;
		case 0xD000: //Draw a sprite at (VX, VY), that has a width of 8 pixels and a height of N pixels
			draw(V[opcode & 0x0F00], V[opcode & 0x00F0], opcode & 0x000F);
			break;

		case 0xE000:
			{
				uint8_t x = (opcode & 0x0F00) / 0x100;
				if (x < 0 || x >= 16) {
					std::cout << "ERROR: invalid register (R" << x << ")" << std::endl;
					exit(1);
				}

				switch (opcode & 0xF0FF) {
					case 0xE09E: //Skip next instruction if the key in VX is pressed
						//TODO
						break;
					case 0xE0A1: //Skip next instruction if the key in VX isn't pressed
						//TODO
						break;
				}
				break;
			}

		case 0xF000:
			{
				uint8_t x = (opcode & 0x0F00) / 0x100;
				if (x < 0 || x >= 16) {
					std::cout << "ERROR: invalid register";
					exit(1);
				}

				switch (opcode & 0x00FF) {
					case 0x0007: //Set VX = delay timer
						V[x] = getDelay();
						break;
					case 0x000A: //Set VX = key press (blocking)
						V[x] = getKey();
						break;
					case 0x0015: //Set delay timer = VX
						setDelay(V[x]);
						break;
					case 0x0018: //Set sound timer = VX
						setSoundTimer(V[x]);
						break;
					case 0x001E: //Set I += Vx
						I += V[x];
						break;
					case 0x0029: //Set I = the location of the sprite for the character in VX
						I = getSpriteAddr(V[x]);
						break;
					case 0x0033: //Store BCD of VX
						setBCD(V[x]);
						//...
						break;
					case 0x0055: //Dump V0 to VX in memory (starting at I)
						dumpRegisters(V[x], I);
						break;
					case 0x0065: //Fill V0 to VX from memory (starting at I)
						loadRegisters(V[x], I);
						break;
					default:
						break;
				}
				break;
			}
		default:
			/* TODO: Refactor the next few opcodes, for 0x3000 to 0x9000 */
			if ((opcode & 0xF000) >= 0x3000 && (opcode & 0xF000) <= 0x9000) {
				short x = (opcode & 0x0F00) / 0x100; //Register index
				uint8_t value = opcode & 0x00FF;
				if (x < 0 || x >= 16) {
					printf("ERROR (op=0x%02X): invalid register (R%i)", opcode, x);
					exit(1);
				}

				switch (opcode & 0xF000) {

					case 0x3000: //Skip next opcode if VX = NN
						if (V[x] == value)
							PC += 2;
						break;
					case 0x4000: //Skip next opcode if VX != NN
						if (V[x] != value)
							PC += 2;
						break;
					case 0x6000: //Set VX = NN
						V[x] = value;
						break;
					case 0x7000: //Set VX += NN (no carry flag)
						V[x] += value;
						break;

					case 0x5000 || 0x8000 || 0x9000: //Refactor for 0x5XY0, 0x8XYN, 0x9XYN
						{
							short x = (opcode & 0x0F00) / 0x100,
								  y = (opcode & 0x00F0) / 0x10;
							if (x < 0 || x >= 16 || y < 0 || y >= 16) {
								std::cout << "ERROR: invalid register";
								exit(1);
							}

							switch (opcode & 0xF00F) {
								case 0x5000: //Skip next opcode if VX == VY
									if (V[x] == V[y])
										PC += 2;
									break;
								/* TODO: Maybe move 0x8 to its own function ? */
								case 0x8000: //Set VX = VY
									V[x] = V[y];
									break;
								case 0x8001: //Set VX = (VX | VY)
									V[x] = (V[x] | V[y]);
									break;
								case 0x8002: //Set VX = (VX & VY)
									V[x] = (V[x] & V[y]);
									break;
								case 0x8003: //Set VX = (VX ^ VY)
									V[x] = (V[x] ^ V[y]);
									break;
								case 0x8004: //Set VX += VY, VF is set to 1 when there's a carry, and to 0 when there isn't
									if ((V[x] + V[y]) > 0xFF) //Since each register is 8 bits...
										V[0xF] = 1;

									V[x] += V[y];
									break;
								case 0x8005: //Set VX -= VY, VF is set to 0 when there's a borrow, and 1 when there isn't
									if ((V[x] - V[y]) < 0) //TODO: Verify
										V[0xF] = 0;

									V[x] -= V[y];
									break;
								case 0x8006: //Shift Vx = Vy >> 1
									//TODO: V[0xF] = LSB of VY (but remember BIG ENDIAN !!)
									V[x] = V[y] >> 1; 
									break;
								case 0x8007: //Set VX = VY - VX, VF is set to 0 when there's a borrow, and 1 when there isn't
									if ((V[y] - V[x]) < 0) //TODO: Verify
										V[0xF] = 0;

									V[x] = V[y] - V[x];
									break;
								case 0x800E: //Shift and set Vx = Vy = Vy << 1
									//TODO: V[0xF] = MSB of V[y] (remember BIG ENDIAND)
									V[y] <<= 1;
									V[x] = V[y];
									break;
								case 0x9000: //Skips the next instruction if VX != VY
									if (V[x] != V[y])
										PC += 2;
									break;
								default:
									break;
							}
							break;
						}
					default:
						break;
				}
			} else {
				printf("[WARNING] Unhandled opcode %02X\n", opcode);
			}
			break;
	}
}
