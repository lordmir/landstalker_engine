#ifndef _TIMER_H_
#define _TIMER_H_

#include <SDL3/SDL_timer.h>
#include <functional>

class Timer
{
public:
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    
    static void Initialise();
    static double GetDelta();
    static void ProcessTicks(std::function<void(unsigned int)> tick_handler);
private:
    Timer();
    static bool init;
    static uint64_t last_timer;
    static uint64_t last_tick;
    static unsigned int total_ticks;
    inline static const double TARGET_TICK_DELTA = 1.0 / 60.0;
};

#endif // _TIMER_H_
