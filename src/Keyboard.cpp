/*
 * Keyboard.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Keyboard.h"
#include "Chip8.h"
#include "Screen.h"
#include "Timer.h"

#include <ncurses.h>
#include <string>
#include <thread>

using namespace std::chrono_literals;

Keyboard::Keyboard(Chip8* chip)
{
  mChip = chip;
  mListening = false;
  for (int i = 0; i < 16; i++)
    mKeys[i] = false;
}

Keyboard::~Keyboard() {}

uint8_t
Keyboard::ReadKey()
{
  bool noKeyPress = true;
  uint8_t key;
  while (noKeyPress && mListening) {
    for (int i = 0; i < 16; i++) {
      if (mKeys[i]) {
        key = i;
        noKeyPress = false;
      }
    }
  }

  return key;
}

/* TODO: Hey why not writing a cool library / ncurses wrapper for asynchronous
 * & keyboard input events (nice lil callbacks and shit). Yeah cause we all know
 * how much of a pain in the ass it can be....
 */
void
Keyboard::ListenerThread()
{
  Timer timer;
  int c;
  std::string mapping("0123456789abcdef"); // Maps a character to a key index
  while (mListening) {
    /* Reset key states */
    for (int i = 0; i < 16; i++) {
      if (mKeys[i] && timer.ElapsedMilliseconds() >= 170) {
        mKeys[i] = false;
        timer.Stop();
        timer.Reset();
      }
    }

    /* Multiple keys input handling :) */
    while ((c = getch()) != ERR && mListening) {
      if (c == 27 || c == 'q') // ESC or Q
        mChip->Stop();

      mKeys[mapping.find(c)] = true;
      timer.Start();
    }
  }
}

void
Keyboard::StartListening()
{
  if (!mListening) {
    mListening = true;
    tListener = std::thread(&Keyboard::ListenerThread, this);
    tListener.join();
  }
}

void
Keyboard::StopListening()
{
  mListening = false;
}

bool
Keyboard::IsKeyPressed(uint8_t x)
{
  return mKeys[x];
}
