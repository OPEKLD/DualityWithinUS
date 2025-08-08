#pragma once

#include "SDL.h"
#include <functional>
#include <vector>
#include <list>
#include <iostream>

struct Timer{
	Uint32 end_time;
	std::function<void()> callbackFunc;
	int id;
};

class TimeManager
{
public:
	TimeManager() = default;
	int AddTimer(Uint32 ms, std::function<void()> callback);
	Uint32 TimeRemaining(int id);
	void update();
private:
	std::list<Timer> timers;
	int uniqueId = 0;
};