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
    memset(mStack, 0, STACK_SIZE * sizeof(uint16_t));
    mSp = 0;
}

uint16_t
Stack::Pop()
{
    uint16_t word = mStack[mSp];
    mStack[mSp--] = 0;

    return word;
}

void
Stack::Push(uint16_t word)
{
    if (mSp > 0)
        mSp++;
    mStack[mSp] = word;
}
