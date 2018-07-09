/*
 * Memory.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "Memory.h"

Memory *Memory::instance = 0;

Memory::Memory() {
    memset(locations, 0, MEMORY_SIZE);
}

Memory::~Memory() {
}

uint8_t Memory::readByte(uint16_t addr) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
	printf("[!] ERROR: address out of bounds\n");
	exit(1);
    }

    return locations[addr];
}

uint16_t Memory::readOpCode(uint16_t addr) {
    if (addr < 0 || addr >= MEMORY_SIZE)  {
	printf("[!] ERROR: address out of bounds\n");
	exit(1);
    }

    /* Shift the first byte 8 bits to the left and OR the following byte */
    return (locations[addr] << 8) | locations[addr + 1];
}

bool Memory::writeByte(uint16_t addr, const uint8_t byte) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
	printf("[!] ERROR: address out of bounds\n");
	return false;
    } else {
	locations[addr] = byte;
	return true;
    }
}

bool Memory::writeBytes(uint16_t addr, size_t length, const uint8_t *bytes) {
    if (addr < 0 || addr >= MEMORY_SIZE) {
        printf("[!] ERROR: address out of bounds\n");
        return false;
    } else if ((length + addr) >= MEMORY_SIZE) {
        printf("[!] ERROR: not enough memory\n");
        return false;
    } else {
	memcpy(&locations[addr], bytes, length); //Write at addr offset

        return true;
    }
}

Memory *Memory::getInstance() {
    if (!instance) {
	instance = new Memory();
    }

    return instance;
}
