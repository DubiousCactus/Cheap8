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

class Stack
{

private:
  uint16_t mStack[STACK_SIZE]; // Size of an address: 2 bytes
  uint16_t mSp;                // Stack pointer: between 0 and STACK_SIZE
  uint16_t mTop;               // Points at the top of the stack

public:
  Stack();
  uint16_t Pop();
  uint16_t Push(uint16_t word);
  uint16_t Fetch(); // Returns word at &sp
};

#endif /* !STACK_H */
