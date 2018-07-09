/*
 * Memory.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <cstdint>

#define MEMORY_SIZE 0xFFF
#define GAME_OFFSET 0x200

class Memory {

	private:
		uint8_t locations[MEMORY_SIZE];
		static Memory *instance;

		Memory();

	public:
		~Memory();
		static Memory *getInstance();
		uint8_t readByte(uint16_t addr);
		uint16_t readOpCode(uint16_t addr);
		bool writeByte(uint16_t addr, uint8_t byte);
		bool writeBytes(uint16_t addr, size_t length, const uint8_t *bytes);
};

#endif /* !MEMORY_H */
