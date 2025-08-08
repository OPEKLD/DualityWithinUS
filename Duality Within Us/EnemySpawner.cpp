#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(Manager* mgr, Entity* plr, Collision* cls, TimeManager* tm)
{
	this->mgr = mgr;
	this->plr = plr;
	this->cls = cls;
	this->tm = tm;
	wave = 1;
	enemyWaves[1] = {
		Vector2D(768, 128),
		Vector2D(1750, 584),
		Vector2D(3302, 384),
		Vector2D(4038, 128),
		Vector2D(2132, 88),
		Vector2D(4138, 528),
		Vector2D(4868, 328),
		Vector2D(1550, 328),
		Vector2D(608, 450),
		Vector2D(5188, 88)
	};
	enemyWaves[2] = {
		Vector2D(768, 128),
		Vector2D(1750, 584),
		Vector2D(3302, 384),
		Vector2D(4038, 128),
		Vector2D(2132, 88),
		Vector2D(4138, 528),
		Vector2D(4868, 328),
		Vector2D(1550, 328),
		Vector2D(608, 450),
		Vector2D(5188, 88),
		Vector2D(4900, 30),
		Vector2D(360, 300),
		Vector2D(3302, 58),
		Vector2D(2000, 428),
		Vector2D(4868, 228)
	};
	enemyWaves[3] = {
		Vector2D(768, 128),
		Vector2D(1750, 584),
		Vector2D(3302, 384),
		Vector2D(4038, 128),
		Vector2D(2132, 88),
		Vector2D(4138, 528),
		Vector2D(4868, 328),
		Vector2D(1550, 328),
		Vector2D(608, 450),
		Vector2D(5188, 88),
		Vector2D(4900, 30),
		Vector2D(360, 300),
		Vector2D(3302, 58),
		Vector2D(2000, 428),
		Vector2D(4868, 228),
		Vector2D(2952, 58),
		Vector2D(738, 128),
		Vector2D(5138, 88),
		Vector2D(4088, 128),
		Vector2D(658, 450)
	};
}

void EnemySpawner::EnemyUpdate()
{
	/*for (auto& e : enemies)
	{
		cls->checkCollisions(*e);
		if (e->getComponent<EnemyComponent>().isDying())
		{
			std::cout << "Enemy was destroyed and needs to be removed" << std::endl;
			enemiesRemaining--;
		}
	}*/
	if (enemiesRemaining > 0)
	{
		for (auto it = enemies.begin(); it != enemies.end();)
		{
			cls->checkCollisions(**it);
			if ((*it)->getComponent<EnemyComponent>().isDying())
			{
				(*it)->destroy();
				it = enemies.erase(it);
				enemiesRemaining--;
				std::cout << enemiesRemaining << std::endl;
			}
			else
			{
				++it;
			}
		}
	}
	else if (!waitingForNextWave)
	{	
		tm->AddTimer(3000, [this]()
		{
			this->NextWave();
		});
		waitingForNextWave = true;
	}

	if (plr->hasComponent<HomingAttackComponent>())
	{
		plr->getComponent<HomingAttackComponent>().readEnemies(enemies);
	}
}

void EnemySpawner::EnemyDestroyed()
{
	enemiesRemaining--;
}

void EnemySpawner::NextWave()
{
	wave++;

	if (wave <= 3)
	{
		SpawnEnemies();
		enemiesRemaining = enemyWaves[wave].size();
		waitingForNextWave = false;
		strongEnemies -= 2;
	}
	else
	{
		gameFinished = true;
	}
}

void EnemySpawner::SpawnEnemies()
{
	if (enemyWaves.find(wave) != enemyWaves.end())
	{
		int count = 1;
		for (const auto& pos : enemyWaves[wave])
		{
			auto& enemy(mgr->addEntity());
			enemy.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
			enemy.addComponent<EnemyComponent>(false, pos.x, (pos.x - 350), 2, plr);
			if (count % strongEnemies == 0)
			{
				enemy.addComponent<SpriteComponent>("Assets/Enemies/AstrocytmareZe.png");
				enemy.addComponent<HealthComponent>(75, 75, tm);
			}
			else
			{
				enemy.addComponent<SpriteComponent>("Assets/Enemies/AstrocytmareDi.png");
				enemy.addComponent<HealthComponent>(25, 25, tm);
			}
			enemy.addComponent<ColliderComponent>("enemy");
			enemies.push_back(&enemy);
			count++;
		}
	}
}
