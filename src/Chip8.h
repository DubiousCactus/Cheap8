/*
 * Chip8.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef CHIP8_H
#define CHIP8_H

#include "CPU.h"
#include "Memory.h"
#include <cstdint>

class Chip8
{

  private:
    bool mRunning;
    uint8_t mKey[16]; // Hex based keypad
    CPU* mCpu;
    Memory* mRam;

  public:
    Chip8();
    ~Chip8();
    void Init();
    void Cycle();
    void DrawGraphics();
    void SetKeys();
    void MainLoop();
    void Run();
    void Stop();
    void Load(const char* file_name);
};

#endif /* !CHIP8_H */