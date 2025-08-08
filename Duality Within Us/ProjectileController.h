#pragma once

#include <vector>
#include "ECS.h"
#include "Components.h"
#include "TimeManager.h"
#include "TextureManager.h"
#include "Vector2D.h"

class ProjectileController
{
public:
	Manager* manager;
	Entity* player;
	TimeManager* tm;

	ProjectileController(Manager* mgr, Entity* plr, TimeManager* tm);

	void update();

	void Spawn(Vector2D pos, int direction);

	void DestroyProjectile();

private:
	KeyboardController* kc;
	std::vector<Entity*> projectiles;
	const char* path = "Assets/Projectiles/ProjectileD.png";
};