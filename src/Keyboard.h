/*
 * Keyboard.h
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <ncurses.h>
#include <cstdint>

class Keyboard
{

  private:
	bool mKeys[16];
	bool mListening;
	WINDOW* mWindow;
	static Keyboard* mInstance;
	Keyboard();
	void ListenerThread();

  public:
	~Keyboard();
    uint8_t ReadKey();
	static Keyboard* GetInstance();
	void StartListening();
	void StopListening();
	bool IsKeyPressed(uint8_t x);
};

#endif /* !KEYBOARD_H */
