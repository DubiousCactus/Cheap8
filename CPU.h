/*
 * CPU.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CPU_H
#define CPU_H

#include "Memory.h"
#include "Stack.h"
#include <cstdint>

#define SCREEN_W 64
#define SCREEN_H 32
#define GAME_OFFSET 0x200

class CPU {

private:
  /* V0-VF (4 bits index), 8-bit size each */
  uint8_t V[16];   // Registers
  uint16_t *I;     // Address register, 16 bits wide
  uint16_t PC;     // Program counter, 16 bits
  uint16_t opcode; // 2 bytes
  uint8_t screen[SCREEN_W * SCREEN_H];
  Memory *ram;  // TODO: use smart pointers
  Stack *stack; // Must be at least of size 16
  uint8_t delay_timer;
  uint8_t sound_timer;
  bool JMP;

  void execute();
  void clearScreen();
  void draw(const uint8_t x, const uint8_t y, const uint8_t height);
  uint8_t getDelay();
  uint8_t getKey();
  void setDelay(uint8_t value);
  void setSoundTimer(uint8_t value);
  uint16_t *getSpriteAddr(uint8_t register);
  void dumpRegisters(int r_offset, uint16_t *addr);
  void loadRegisters(int r_offset, uint16_t *addr);
  void setBCD(uint8_t register);

public:
  CPU();
  ~CPU();
  void step();
};

#endif /* !CPU_H */
