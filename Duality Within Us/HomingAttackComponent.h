#pragma once

#include <vector>
#include "TimeManager.h"
#include "ECS.h"
#include "Components.h"
#include "HealthComponent.h"
#include "EnemyComponent.h"

class HomingAttackComponent : public Component
{
public:

	HomingAttackComponent(TimeManager* tm)
	{
		this->tm = tm;
	}

	void readEnemies(std::vector<Entity*> ene)
	{
		enemies = ene;
	}

	void update() override
	{
		float closestDistanceSquared = range * range;

		if (!enemies.empty())
		{
			for (auto& e : enemies)
			{
				float dx = e->getComponent<TransformComponent>().position.x - entity->getComponent<TransformComponent>().position.x;
				float dy = e->getComponent<TransformComponent>().position.y - entity->getComponent<TransformComponent>().position.y;

				float distSq = dx * dx + dy * dy;

				if (distSq < closestDistanceSquared && e->getComponent<HealthComponent>().hp <= 25 && e->getComponent<EnemyComponent>().isDying() != true)
				{
					nearest = e;
					e->getComponent<SpriteComponent>().targetted = true;
				}
				else if (distSq > closestDistanceSquared)
				{
					e->getComponent<SpriteComponent>().targetted = false;
					if (nearest != nullptr && nearest == e)
					{
						nearest = nullptr;
					}
				}
			}
		}

		
	}

	void attack()
	{
		if (nearest && nearest != nullptr)
		{
			float dx = nearest->getComponent<TransformComponent>().position.x - entity->getComponent<TransformComponent>().position.x;
			float dy = nearest->getComponent<TransformComponent>().position.y - entity->getComponent<TransformComponent>().position.y;
			float magnitude = std::sqrt(dx * dx + dy * dy);

			if (magnitude != 0)
			{
				dx /= magnitude;
				dy /= magnitude;
			}

			float speed = 7.5f;
			entity->getComponent<TransformComponent>().velocity.x += dx * speed;
			entity->getComponent<TransformComponent>().velocity.y += dy * speed;

			nearest->getComponent<HealthComponent>().takeDamage(26, 0);
			nearest->getComponent<EnemyComponent>().changeState(2);

			tm->AddTimer(200, [this]() {
				this->stopMovement();
			});

			nearest = nullptr;
		}
	}

	void stopMovement()
	{
		entity->getComponent<TransformComponent>().velocity.x = 0;
		entity->getComponent<TransformComponent>().velocity.y = 0;
	}

private:
	TimeManager* tm;
	std::vector<Entity*> enemies;
	Entity* nearest;
	float range = 300;
};