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
    mWindow = newwin(SCREEN_HEIGHT + 2, SCREEN_WIDTH + 2, 5, 5);
	nodelay(mWindow, TRUE); // <- de-blocking the user input
	box(mWindow, 0, 0);
}

Screen::~Screen()
{
	delwin(mWindow);
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
    memset(mBuffer, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(mBuffer[0][0]));
}

bool
Screen::TogglePixel(const uint8_t x, const uint8_t y)
{
    bool collision = mBuffer[x][y] == 1 ? true  : false;
    mBuffer[x][y] ^= 1;

    return collision;
}

void
Screen::Draw()
{
    for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y = 0; y < SCREEN_HEIGHT; y++) {
			mvwaddch(mWindow, y + 1, x + 1, mBuffer[x][y] == 1 ? ACS_BLOCK : ' ');
		}
    }
    wrefresh(mWindow);
}

WINDOW*
Screen::GetHandle()
{
    return mWindow;
}

void
Screen::Resize()
{
	// TODO
	int nh, nw;
	getmaxyx(stdscr, nh, nw);  /* get the new screen size */
}
