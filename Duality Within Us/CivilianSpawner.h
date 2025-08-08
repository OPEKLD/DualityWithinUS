#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collisions.h"
#include "TimeManager.h"
#include <unordered_map>
#include <vector>
#include <string>

class CivilianSpawner
{
public:
	CivilianSpawner(Manager* mgr, Collision* cls, TimeManager* tm, Entity* player);
	void update();
	void spawn();
private:
	Manager* mgr;
	Collision* cls;
	TimeManager* tm;
	Entity* player;
	std::vector<Entity*> civilians;
	std::unordered_map<std::string, std::vector<Vector2D>> civilianData;
};