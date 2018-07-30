/*
 * Screen.h
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef SCREEN_H
#define SCREEN_H

#include <cstdint>
#include <ncurses.h>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32

class Screen
{
  private:
    static Screen* mInstance;
	Screen();
    uint8_t mBuffer[SCREEN_WIDTH][SCREEN_HEIGHT];
    WINDOW* mWindow;

  public:
	~Screen();
    static Screen* GetInstance();
    bool TogglePixel(const uint8_t x, const uint8_t y);
    void Draw();
	void Clear();
	WINDOW* GetHandle();
};

#endif /* !SCREEN_H */
