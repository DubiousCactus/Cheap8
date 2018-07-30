/*
 * Timer.cpp
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#include "Timer.h"

void
Timer::Start()
{
    mStartTime = std::chrono::system_clock::now();
    mRunning = true;
}

void
Timer::Stop()
{
    mEndTime = std::chrono::system_clock::now();
    mRunning = false;
}

void
Timer::Reset()
{
	mStartTime = std::chrono::system_clock::now();
}

double
Timer::ElapsedNanoseconds()
{
    std::chrono::time_point<std::chrono::system_clock> endTime;
    endTime = mRunning ? std::chrono::system_clock::now() : mEndTime;

    return std::chrono::duration_cast<
	std::chrono::nanoseconds
    >(endTime - mStartTime).count();
}

double
Timer::ElapsedMilliseconds()
{
    return ElapsedNanoseconds() / 1000000.0;
}

double
Timer::ElapsedSeconds()
{
    return ElapsedMilliseconds() / 1000.0;
}
