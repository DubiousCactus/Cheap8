/*
 * Memory.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>
#include <cstdint>

#define MEMORY_SIZE 0x1000
#define GAME_OFFSET 0x200

class Memory
{

private:
  uint8_t mLocations[MEMORY_SIZE];
  static Memory* mInstance;

  Memory();

public:
  ~Memory();
  static Memory* GetInstance();
  uint8_t ReadByte(uint16_t addr);
  uint16_t ReadOpCode(uint16_t addr);
  bool WriteByte(uint16_t addr, uint8_t byte);
  bool WriteBytes(uint16_t addr, std::size_t length, const uint8_t* bytes);
};

#endif /* !MEMORY_H */
