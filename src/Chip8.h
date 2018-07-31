/*
 * Chip8.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CHIP8_H
#define CHIP8_H

#include "CPU.h"

#include <cstdint>

class Chip8
{

private:
  bool mRunning;
  CPU* mCpu;
  Memory* mRam;
  Screen* mScreen;
  Keyboard* mKeyboard;

  void UpdateTimers();
  void Init();
  void CPULoop();
  void UILoop();

public:
  Chip8(Screen* screen);
  ~Chip8();
  void Run();
  void Stop();
  void Load(const char* file_name);
};

#endif /* !CHIP8_H */
