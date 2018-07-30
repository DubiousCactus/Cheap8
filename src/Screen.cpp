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
    /* ncurses */
    initscr();
    cbreak();
    noecho();
    /* Clear the buffer */
    Clear();
    mWindow = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
}

Screen::~Screen()
{
    endwin();
}

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

WINDOW*
Screen::GetHandle()
{
    return mWindow;
}
