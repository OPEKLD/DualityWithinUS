#pragma once

#include "ECS.h"
#include "Components.h"
#include "Vector2D.h"
#include "Collisions.h"
#include <unordered_map>
#include <vector>

class EnemySpawner
{
public:
	bool gameFinished = false;

	EnemySpawner(Manager* mgr, Entity* plr, Collision* cls, TimeManager* tm);
	void EnemyUpdate();
	void EnemyDestroyed();
	void NextWave();
	void SpawnEnemies();

private:
	Manager* mgr;
	Entity* plr;
	Collision* cls;
	TimeManager* tm;
	std::vector<Entity*> enemies;
	std::unordered_map<int, std::vector<Vector2D>> enemyWaves;
	int wave = 0;
	int enemiesRemaining = 10;
	int strongEnemies = 5;
	bool waitingForNextWave = false;
};