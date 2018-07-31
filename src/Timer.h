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
  std::chrono::time_point<std::chrono::high_resolution_clock> mStartTime;
  std::chrono::time_point<std::chrono::high_resolution_clock> mEndTime;
  bool mRunning = false;

public:
  void Start();
  void Stop();
  void Reset();
  double ElapsedSeconds();
  double ElapsedMilliseconds();
  double ElapsedNanoseconds();
};

#endif /* !TIMER_H */
