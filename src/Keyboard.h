/*
 * Keyboard.h
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <cstdint>

class Keyboard
{

  private:
	bool mKeys[16];
	static Keyboard* mInstance;
	Keyboard();

  public:
	~Keyboard();
    uint8_t ReadKey();
	static Keyboard* GetInstance();
};

#endif /* !KEYBOARD_H */
