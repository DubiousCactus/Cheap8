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

void
Keyboard::ListenerThread()
{
  Timer timer;
  int c;
  std::string mapping("0123456789abcdef");
  while (mListening) {
    for (int i = 0; i < 16; i++) {
      if (mKeys[i] && timer.ElapsedMilliseconds() >= 5) {
        mKeys[i] = false;
        timer.Stop();
        timer.Reset();
      }
    }

    c = getch();
    if (c != ERR) {
      if (c == 27 || c == 'q') // ESC or Q
        mChip->Stop();

      int keyIndex = (int)mapping.find(c);
      if (keyIndex >= 0) {
        mKeys[keyIndex] = true;
        timer.Start();
      }
    }
    // std::this_thread::sleep_for(1ms);
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
