/*
 * Stack.h
 * Copyright (C) 2018 transpalette <transpalette@arch-cactus>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef STACK_H
#define STACK_H

#include <cstdint>

#define STACK_SIZE 48

class Stack {

	private:
		uint16_t stack[STACK_SIZE]; //Size of an address: 2 bytes
		uint16_t sp; //Stack pointer: between 0 and STACK_SIZE
		uint16_t top; //Points at the top of the stack

	public:
		Stack();
		uint16_t pop();
		uint16_t push(uint8_t byte);
		uint16_t fetch(); //Returns byte at &sp
};

#endif /* !STACK_H */
