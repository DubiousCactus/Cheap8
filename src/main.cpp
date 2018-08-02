/*
 * main.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Chip8.h"
#include "Screen.h"

#include <csignal>
#include <cstdio>
#include <signal.h>

Screen* screen;
Chip8* cheap8;

void
ResizeHandler(int sig)
{
  screen->Resize();
}

void
QuitHandler(int sig)
{
  cheap8->Stop();
  screen->Terminate();
}

int
main(int argc, char** argv)
{
  struct sigaction sigIntHandler, sigWinchHandler;

  sigIntHandler.sa_handler = QuitHandler;
  sigemptyset(&sigIntHandler.sa_mask);
  sigIntHandler.sa_flags = 0;

  sigWinchHandler.sa_handler = ResizeHandler;
  sigemptyset(&sigWinchHandler.sa_mask);
  sigWinchHandler.sa_flags = 0;

  sigaction(SIGINT, &sigIntHandler, NULL);
  sigaction(SIGWINCH, &sigWinchHandler, NULL);

  if (argc < 2) {
    printf("Usage: %s <chip8_game_image>\n\n", argv[0]);
    return 0;
  }

  screen = new Screen();
  cheap8 = new Chip8(screen);
  cheap8->Load(argv[1]);
  cheap8->Run();

  delete screen;

  return 0;
}
