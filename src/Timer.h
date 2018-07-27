/*
 * Timer.h
 * Copyright (C) 2018 transpalette <transpalette@translaptop>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
  private:
    std::chrono::time_point<std::chrono::system_clock> mStartTime;
    std::chrono::time_point<std::chrono::system_clock> mEndTime;
    bool mRunning = false;

  public:
    void Start();
    void Stop();
    double ElapsedSeconds();
    double ElpasedMilliseconds();
};

#endif /* !TIMER_H */
