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

double
Timer::ElpasedMilliseconds()
{
    std::chrono::time_point<std::chrono::system_clock> endTime;
    endTime = mRunning ? std::chrono::system_clock::now() : mEndTime;

    return std::chrono::duration_cast
		<std::chrono::milliseconds
	>(endTime - mStartTime).count();
}

double
Timer::ElapsedSeconds()
{
    return ElpasedMilliseconds() / 1000.0;
}
