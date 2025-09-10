#include "timeplayed.h"

Timer::Timer()
{
    init_time = std::chrono::steady_clock::now();
}

inline int Timer::GetTime()
{
    std::chrono::time_point<std::chrono::steady_clock> now = std::chrono::steady_clock::now();
    return std::chrono::duration<double>(now - init_time).count();
}

int Timer::GetSeconds()
{
    return GetTime() % 60;
}

int Timer::GetMinutes()
{
    return GetTime() / 60 % 60;
}

int Timer::GetHours()
{
    return GetTime() / 3600;
}
