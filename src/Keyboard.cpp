/*
 * Keyboard.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Keyboard.h"

Keyboard* Keyboard::mInstance = nullptr;

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

Keyboard*
Keyboard::GetInstance()
{
    if (!Keyboard::mInstance)
		Keyboard::mInstance = new Keyboard();

    return Keyboard::mInstance;
}
