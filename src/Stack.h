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
  uint16_t mSp;               // Points at the top of the stack

public:
  Stack();
  uint16_t Pop();
  void Push(uint16_t word);
};

#endif /* !STACK_H */
