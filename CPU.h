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

class CPU
{

private:
  /* V0-VF (4 bits index), 8-bit size each */
  uint8_t mV[16];   // Registers
  uint16_t mI;      // Address register, 16 bits wide
  uint16_t mPC;     // Program counter, 16 bits
  uint16_t mOpcode; // 2 bytes
  uint8_t mScreen[SCREEN_W * SCREEN_H];
  Memory* mRam;  // TODO: use smart pointers
  Stack* mStack; // Must be at least of size 16
  uint8_t mDelay_timer;
  uint8_t mSound_timer;
  bool mJMP;

  void Execute();
  void ClearScreen();
  void Draw(const uint8_t x, const uint8_t y, const uint8_t height);
  uint8_t GetDelay();
  uint8_t GetKey();
  void SetDelay(uint8_t value);
  void SetSoundTimer(uint8_t value);
  uint16_t GetSpriteAddr(uint8_t register);
  void DumpRegisters(int r_offset, uint16_t addr);
  void LoadRegisters(int r_offset, uint16_t addr);
  void SetBCD(uint8_t register);

public:
  CPU();
  ~CPU();
  void Step();
};

#endif /* !CPU_H */
