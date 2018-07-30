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

#include <ncurses.h>
#include <cstdint>

class Chip8
{

  private:
    bool mRunning;
    uint8_t mKey[16]; // Hex based keypad
    uint8_t mDelay_timer;
    uint8_t mSound_timer;
    CPU* mCpu;
    Memory* mRam;
    WINDOW* mWindow;
    void UpdateTimers();
    void DrawGraphics();
    void Init();
    void Cycle();

  public:
    Chip8();
    ~Chip8();
    void MainLoop();
    void Run();
    void Stop();
    void Load(const char* file_name);
};

#endif /* !CHIP8_H */
