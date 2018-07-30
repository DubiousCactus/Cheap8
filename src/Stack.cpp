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
    uint16_t word = mStack[mTop];
    mStack[mTop--] = 0;
    if (mSp > mTop)
        mSp = mTop;

    return word;
}

uint16_t
Stack::Push(uint16_t word)
{
    mStack[++mTop] = word;

    return mTop;
}

uint16_t
Stack::Fetch()
{
    printf("[*] Fetching word from the stack (SP=%02X)\n", mSp);

    return mStack[mSp];
}
