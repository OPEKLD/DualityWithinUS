#include "TimeManager.h"

int TimeManager::AddTimer(Uint32 ms, std::function<void()> callback)
{
	Timer timer;
	Uint32 now = SDL_GetTicks();
	timer.end_time = ms + now;
	timer.callbackFunc = callback;
	timer.id = uniqueId;
	timers.push_back(timer);
	uniqueId++;

	return timer.id;
}

Uint32 TimeManager::TimeRemaining(int id)
{
	Uint32 returnValue;
	auto it = timers.begin();
	while (it != timers.end())
	{
		if ((*it).id == id)
		{
			returnValue = (*it).end_time - SDL_GetTicks();
			break;
		}
	}
	return returnValue;
}

void TimeManager::update()
{
	Uint32 now = SDL_GetTicks();

	auto it = timers.begin();
	while (it != timers.end())
	{
		if (now >= it->end_time)
		{
			it->callbackFunc();
			it = timers.erase(it);
		}
		else
		{
			++it;
		}
	}
}
