/*
 * CPU.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CPU_H
#define CPU_H

#include "Memory.h"
#include "Screen.h"
#include "Stack.h"
#include "Keyboard.h"

#include <cstdint>

class CPU
{

private:
  /* V0-VF (4 bits index), 8-bit size each */
  uint8_t mV[16];   // Registers
  uint16_t mI;      // Address register, 16 bits wide
  uint16_t mPC;     // Program counter, 16 bits
  uint16_t mOpcode; // 2 bytes
  Memory* mRam;     // TODO: use smart pointers
  Keyboard* mKeyboard;
  Stack* mStack; // Must be at least of size 16
  Screen* mScreen;
  uint8_t mDelay_timer;
  uint8_t mSound_timer;
  bool mJMP;

  void Execute();
  void Draw(uint8_t x, uint8_t y, uint8_t height);
  uint8_t GetDelay();
  void SetDelay(uint8_t value);
  void SetSoundTimer(uint8_t value);
  uint16_t GetSpriteAddr(uint8_t r);
  void DumpRegisters(int r_index, uint16_t addr);
  void LoadRegisters(int r_index, uint16_t addr);
  void SetBCD(uint8_t r);

public:
  CPU(Keyboard* keyboard, Memory* ram, Screen* screen);
  ~CPU();
  void Step();
  void UpdateTimers();
};

#endif /* !CPU_H */
