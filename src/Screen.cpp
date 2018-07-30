/*
 * Screen.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Screen.h"

#include <cstring>

Screen* Screen::mInstance = nullptr;

Screen::Screen()
{
    Clear();
}

Screen::~Screen() {}

Screen*
Screen::GetInstance()
{
    if (!Screen::mInstance)
		Screen::mInstance = new Screen();

    return Screen::mInstance;
}

void
Screen::Clear()
{
    memset(mBuffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint8_t));
}

void
Screen::SetWindow(WINDOW* win)
{
    mWindow = win;
}

bool
Screen::TogglePixel(const uint8_t x, const uint8_t y)
{
    bool collision = mBuffer[x][y];
    mBuffer[x][y] ^= mBuffer[x][y];

    return collision;
}

void
Screen::Draw()
{
    for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			mvwaddch(mWindow, x, y, mBuffer[x][y] ? '#' : ' ');
		}
    }
    wrefresh(mWindow);
}
