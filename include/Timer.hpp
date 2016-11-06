#ifndef _TIMER_
#define _TIMER_

#include <chrono>

struct Timer {
  using TimerType = std::chrono::time_point<std::chrono::high_resolution_clock>;

  static TimerType currentTime() { return std::chrono::high_resolution_clock::now(); }

  const int ticksPerSecond = 25;
  const int maxFrameskip = 5;
  int skippedFrames;
  int framesRendered;

  TimerType start;
  TimerType currTime;
  TimerType dt;
  TimerType nextGameTick;
  TimerType lastFpsUpdate;
  TimerType lastFrameRendered;

  std::chrono::milliseconds skipTicks;

  Timer() : 
    skippedFrames(0), framesRendered(0), 
    start(currentTime()), currTime(currentTime()), dt(currentTime()),
    nextGameTick(currentTime()), lastFpsUpdate(currentTime()), lastFrameRendered(currentTime()),
    skipTicks(0)
  {
    skipTicks = std::chrono::milliseconds(1000 / ticksPerSecond);
  }

};

#endif