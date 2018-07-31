/*
 * Chip8.cpp
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#include "Chip8.h"
#include "Keyboard.h"
#include "Screen.h"
#include "Timer.h"

#include <chrono>
#include <condition_variable>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

Chip8::Chip8(Screen* screen)
{
  mRunning = false;
  mScreen = screen;
  mRam = new Memory();
  mKeyboard = new Keyboard(this);
  mCpu = new CPU(mKeyboard, mRam, mScreen);
  Init();
}

Chip8::~Chip8()
{
  delete mCpu;
  delete mScreen;
  // delete mKeyboard;
  // delete mRam;
}

void
Chip8::Init()
{
  /* Init fonts */
  uint8_t fonts[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
  };
  mRam->WriteBytes(FONTS_OFFSET, sizeof(fonts), fonts);
}

void
Chip8::UpdateTimers()
{
  while (this->mRunning) {
    mCpu->UpdateTimers();
    std::this_thread::sleep_for(16ms);
  }
}

void
Chip8::CPULoop()
{
  while (mRunning) {
    /* Run next instruction */
    mCpu->Step();
    if (mCpu->GetDrawFlag()) {
      mDrawAction.notify_one();
      mCpu->ResetDrawFlag();
    }
    /* Run the CPU at 4MHz */
    std::this_thread::sleep_for(2ms);
  }
}

void
Chip8::UILoop()
{
  while (mRunning) {
    /* Don't refresh the UI until the buffer has been updated */
    std::unique_lock<std::mutex> lock(mtx);
    mDrawAction.wait(lock);
    mScreen->Draw();
    /* Try to refresh the screen at a 60Hz rate (max) */
    std::this_thread::sleep_for(10ms);
  }
}

void
Chip8::Run()
{
  if (!mRunning) {
    mRunning = true;
    // mKeyboard->StartListening();
    std::thread tUI(&Chip8::UILoop, this);
    std::thread tCPU(&Chip8::CPULoop, this);
    std::thread tTimers(&Chip8::UpdateTimers, this);
    // mKeyboard->StopListening();

    tUI.join();
    tCPU.join();
    tTimers.join();
  }
}

void
Chip8::Stop()
{
  mRunning = false;
}

/* Load a CHIP-8 program - from a file - into the RAM */
void
Chip8::Load(const char* file_name)
{
  FILE* file = fopen(file_name, "rb");
  if (file == nullptr) {
    std::cout << "[!] ERROR: Could not open '" << file_name << "'" << std::endl;
    exit(1);
  }

  uint8_t* bytes;
  /* Read file size */
  fseek(file, 0, SEEK_END);
  size_t file_size = ftell(file);
  rewind(file);

  /* Read the whole file as a block */
  bytes = (uint8_t*)malloc(sizeof(uint8_t) * file_size);
  size_t size_read = fread(bytes, 1, file_size, file);

  if (file_size > 3584) {
    printf("[!] ERROR: ROM file too large\n");
    exit(1);
  }

  if (size_read != file_size) {
    printf("[!] ERROR: could not read %s\n", file_name);
    exit(1);
  }

  if (!mRam->WriteBytes(0x200, size_read, bytes)) {
    printf("[!] ERROR: could not write the game in the RAM\n");
    exit(1);
  }

  fclose(file);
  free(bytes);
}
