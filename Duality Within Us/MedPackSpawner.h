#pragma once

#include "ECS.h"
#include "Components.h"
#include "Collisions.h"

class MedPackSpawner
{
public:
	MedPackSpawner(Manager* mgr, Collision* cls);
	void update();
	void spawn();
private:
	Manager* mgr;
	Collision* cls;
	std::vector<Entity*> medPacks;
	std::vector<Vector2D> medPackData;
};