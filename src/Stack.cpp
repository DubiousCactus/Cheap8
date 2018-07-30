/*
 * mStack.cpp
 * Copyright (C) 2018 transpalette <transpalette@translapmTop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Stack.h"
#include <cstdio>
#include <cstring>

Stack::Stack()
{
    mTop = 0;
    memset(mStack, 0, STACK_SIZE * sizeof(mStack[0]));
    mSp = 0;
}

uint16_t
Stack::Pop()
{
    return mStack[mTop];
}

uint16_t
Stack::Push(uint8_t byte)
{
    // TODO
    return 0x0;
}

uint16_t
Stack::Fetch()
{
    printf("[*] Fetching byte from the stack (SP=%02X)\n", mSp);
    return mStack[mSp];
}
