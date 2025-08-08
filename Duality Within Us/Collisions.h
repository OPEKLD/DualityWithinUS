#pragma once

#include "SDL.h"
#include "ECS.h"
#include "Components.h"
#include "TimeManager.h"
#include "ProjectileController.h"
#include "Game.h"
#include <map>

class Collision
{
public:
	Manager* mgr;
	TimeManager* tm;

	// Axis-Aligned Bounding Box Collision detection function
	Collision(Manager* mgr, TimeManager* tm);
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	void checkCollisions(Entity& en);

	// Collision Handling functions
	static void PlayerEnemyCollision(Entity* player, Entity* enemy);
	static void EnemyProjectileCollision(Entity* enemy, Entity* projectile);
	static void PlayerWoundedCollision(Entity* player, Entity* civilian);
	static void PlayerMedpackCollision(Entity* player, Entity* medpack);
	static void CivilianProjectileCollision(Entity* civilian, Entity* projectile); 

private:
	using CollisionFunction = void(*)(Entity*, Entity*);
	std::map<std::pair<std::string, std::string>, CollisionFunction> CollisionMatrix;
};