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
	for (int i = 0; i < 16; i++)
	    mKeys[i] = false;
	int c = getch();
	std::string mapping("0123456789ABCDEF");
	if (c != ERR) {
	    if (mapping.find(c) >= 0 && mapping.find(c) <= 'F')
		mKeys[mapping.find(c)] = true;
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
