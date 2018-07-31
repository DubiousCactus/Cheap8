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
  while (mListening) {
    for (int i = 0; i < 16; i++) {
      mKeys[i] = false;
    }

    printf("Reading\n");
    int c = getch();
    std::string mapping("0123456789abcdef");
    if (c != ERR) {
      if (c == 27 || c == 'q') // ESC or Q
        mChip->Stop();

      if (mapping.find(c)) {
        printf("setting key %ld\n", mapping.find(c));
        mKeys[mapping.find(c)] = true;
      }
    }
    std::this_thread::sleep_for(10ms);
  }
}

void
Keyboard::StartListening()
{
  if (!mListening) {
    mListening = true;
    tListener = std::thread(&Keyboard::ListenerThread, this);
  }
}

void
Keyboard::StopListening()
{
  mListening = false;
  tListener.join();
}

bool
Keyboard::IsKeyPressed(uint8_t x)
{
  return mKeys[x];
}
