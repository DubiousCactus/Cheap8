/*
 * Memory.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Memory.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>


/* TODO: TAKE CARE OF THE ENDIANNESS PROBLEM */

Memory* Memory::mInstance = nullptr;

Memory::Memory()
{
  memset(mLocations, 0, MEMORY_SIZE);
}

Memory::~Memory() {}

uint8_t
Memory::ReadByte(uint16_t addr)
{
  if (addr < 0 || addr >= MEMORY_SIZE) {
    printf("[!] ERROR: address out of bounds: 0x%02X [ReadByte]\n", addr);
    exit(1);
  }

  return mLocations[addr];
}

uint16_t
Memory::ReadOpCode(uint16_t addr)
{
  if (addr < 0 || addr >= MEMORY_SIZE) {
    printf("[!] ERROR: address out of bounds: 0x%02X [ReadOpCode]\n", addr);
    exit(1);
  }

  /* Shift the first byte 8 bits to the left and OR the following byte */
  return (mLocations[addr] << 8) | mLocations[addr + 1];
}

bool
Memory::WriteByte(uint16_t addr, const uint8_t byte)
{
  if (addr < 0 || addr >= MEMORY_SIZE) {
    printf("[!] ERROR: address out of bounds [WriteByte]\n");
    return false;
  } else {
    mLocations[addr] = byte;
    return true;
  }
}

bool
Memory::WriteBytes(uint16_t addr, std::size_t length, const uint8_t* bytes)
{
  if (addr < 0 || addr >= MEMORY_SIZE) {
    printf("[!] ERROR: address out of bounds [WriteBytes]\n");
    return false;
  } else if ((length + addr) >= MEMORY_SIZE) {
    printf("[!] ERROR: not enough memory\n");
    return false;
  } else {
    memcpy(&mLocations[addr], bytes, length); // Write at addr offset

    return true;
  }
}

Memory*
Memory::GetInstance()
{
  if (!mInstance) {
    mInstance = new Memory();
  }

  return mInstance;
}
