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
    memset(mStack, 0, STACK_SIZE * sizeof(mStack[0]));
    mSp = 0;
}

uint16_t
Stack::Pop()
{
    uint16_t word = mStack[mSp];
    mStack[mSp--] = 0;

    return word;
}

uint16_t
Stack::Push(uint16_t word)
{
    mStack[++mSp] = word;

    return mSp;
}

