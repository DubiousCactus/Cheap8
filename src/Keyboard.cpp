/*
 * Keyboard.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Keyboard.h"

short
Keyboard::ReadKey()
{
    for (int i = 0; i < 16; i++) {
		if (mKeys[i])
			return i;
    }

    return -1;
}
