/*
 * Stack.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include <cstdio>
#include <cstring>
#include "Stack.h"


Stack::Stack() {
	top = 0;
	memset(stack, 0, STACK_SIZE);
	sp = 0;
}

uint16_t Stack::pop() {
	return stack[top];
}

uint16_t Stack::push(uint8_t byte) {
	//TODO
	return 0x0;
}

uint16_t Stack::fetch() {
	printf("[*] Fetching byte from the stack (SP=%02X)\n", sp);
	return stack[sp];
}
