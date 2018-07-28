/*
 * Keyboard.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Keyboard.h"

#include <thread>
#include <cstdio>

Keyboard* Keyboard::mInstance = nullptr;

Keyboard::Keyboard()
{
	mListening = false;
	for (auto& key : mKeys)
		key = false;
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
		int c = getchar();
		printf("[DEBUG] Reading %c from keyboard\n", c);
		switch (c) {
		}
	}
}

void
Keyboard::StartListening()
{
	if (!mListening) {
		mListening = true;
		std::thread tListener(&Keyboard::ListenerThread, this);
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

Keyboard*
Keyboard::GetInstance()
{
    if (!Keyboard::mInstance)
		Keyboard::mInstance = new Keyboard();

    return Keyboard::mInstance;
}
