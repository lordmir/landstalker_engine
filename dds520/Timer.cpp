#include "Timer.h"

bool Timer::init = false;
uint64_t Timer::last_timer = 0ULL;
uint64_t Timer::last_tick = 0ULL;
unsigned int Timer::total_ticks = 0U;

void Timer::Initialise()
{
    if(!init)
    {
        last_timer = SDL_GetPerformanceCounter();
        last_tick = last_timer;
        total_ticks = 0U;
        init = true;
    }
}

double Timer::GetDelta()
{
    Initialise();
    uint64_t current_timer = SDL_GetPerformanceCounter();
    double delta = (current_timer - last_timer) / static_cast<double>(SDL_GetPerformanceFrequency());
    last_timer = current_timer;
    return delta;
}

void Timer::ProcessTicks(std::function<void(unsigned int)> tick_handler)
{
    Initialise();
    uint64_t current_timer = SDL_GetPerformanceCounter();
    double tick_delta = (current_timer - last_tick) / static_cast<double>(SDL_GetPerformanceFrequency());
    unsigned int ticks_to_process = static_cast<unsigned int>(tick_delta / TARGET_TICK_DELTA) - total_ticks;
    for (unsigned int i = 0; i < ticks_to_process; ++i)
    {
        tick_handler(total_ticks);
        ++total_ticks;
    }
}