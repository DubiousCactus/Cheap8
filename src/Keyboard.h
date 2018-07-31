/*
 * Keyboard.h
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <atomic>
#include <cstdint>
#include <ncurses.h>
#include <thread>

class Chip8; // Forward-declaration

class Keyboard
{

private:
  bool mKeys[16];
  std::atomic<bool> mListening;
  Chip8* mChip;
  std::thread tListener;

  void ListenerThread();

public:
  Keyboard(Chip8* chip);
  ~Keyboard();
  uint8_t ReadKey();
  void StartListening();
  void StopListening();
  bool IsKeyPressed(uint8_t x);
};

#endif /* !KEYBOARD_H */
