/*
 * Keyboard.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Keyboard.h"
#include "Screen.h"

#include <ncurses.h>
#include <thread>

Keyboard* Keyboard::mInstance = nullptr;

Keyboard::Keyboard()
{
    mListening = false;
    for (int i = 0; i < 16; i++)
	mKeys[i] = false;
}

Keyboard::~Keyboard()
{
    delete mInstance;
}

uint8_t
Keyboard::ReadKey()
{
    bool noKeyPress = true;
    uint8_t key;
    while (noKeyPress) {
	for (int i = 0; i < 16; i++) {
	    if (Keyboard::mKeys[i]) {
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
	nodelay(mWindow, TRUE); // <- de-blocking
	int c = getch();	// Blocking!
	switch (c) {
	    case ERR: // No input
		break;
	    case '1':
		break;
	    case '2':
		break;
	    case '3':
		break;
	    case '4':
		break;
	    case '5':
		break;
	    case '6':
		break;
	}
    }
}

void
Keyboard::StartListening()
{
    if (!mListening) {
	mListening = true;
	mWindow = Screen::GetInstance()->GetHandle();
	std::thread(&Keyboard::ListenerThread, this).detach();
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

Keyboard*
Keyboard::GetInstance()
{
    if (!Keyboard::mInstance)
	Keyboard::mInstance = new Keyboard();

    return Keyboard::mInstance;
}
