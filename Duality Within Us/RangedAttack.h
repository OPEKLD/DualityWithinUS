#pragma once

#include "Game.h"
#include "ECS.h"
#include "Components.h"

class RangedAttack : public Component
{
public:
	RangedAttack(int dir)
	{
		direction = dir;
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
	}

	void update() override
	{
		transform->velocity.x = 2 * direction;
	}

private:
	TransformComponent* transform;
	int direction;
};