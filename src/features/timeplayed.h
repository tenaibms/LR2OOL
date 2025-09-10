#pragma once
#include <chrono>

class Timer {
private:
	std::chrono::time_point<std::chrono::steady_clock> init_time;
public:
	/* realistically, no one is going to run lr2 for more than INT_MAX seconds... */
	Timer();
	~Timer() = default;
	inline int GetTime();
	int GetSeconds();
	int GetMinutes();
	int GetHours();
};