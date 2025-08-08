#pragma once

#include "ECS.h"
#include "Components.h"

class EnemyComponent : public Component
{
public:
	void init() override
	{
		state = State::patrol;
		tc = &entity->getComponent<TransformComponent>();
	}

	EnemyComponent(bool hc, int sx, int ex, int sp, Entity* player)
	{
		hasCivilian = hc;
		startX = sx;
		endX = ex;
		speed = sp;
		this->player = player;
		playerTransform = &this->player->getComponent<TransformComponent>();
	}

	void changeState(int num)
	{
		state = static_cast<State>(num);
	}

	bool isDying()
	{
		if (state == State::dying)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void update() override
	{
		switch (state)
		{
		case State::patrol:
			Patrol();
			break;
		case State::chase:
			tc->velocity.x = 0;
			tc->velocity.y = 0;
			Chase();
			break;
		case State::dying:
			Dying();
		}
	}

	void Patrol()
	{
		tc->speed = speed;
		if (movingLeft == true)
		{
			tc->velocity.x = -1;
			if (tc->position.x <= endX)
			{
				movingLeft = false;
			}
		}
		else
		{
			tc->velocity.x = 1;
			if (tc->position.x >= startX)
			{
				movingLeft = true;
			}
		}

		float dx = playerTransform->position.x - tc->position.x;
		float dy = playerTransform->position.y - tc->position.y;
		float distance = std::sqrt(dx * dx + dy * dy);

		if (distance < aggro)
		{
			state = State::chase;
		}
	}

	void Chase()
	{
		float dx = playerTransform->position.x - tc->position.x;
		float dy = playerTransform->position.y - tc->position.y;
		float length = std::sqrt(dx * dx + dy * dy);

		if (length != 0)
		{
			dx /= length;
			dy /= length;
		}

		tc->velocity.x += dx * (float)speed;
		tc->velocity.y += dy * (float)speed;
	}

	void Dying()
	{
		tc->velocity.x = 0;
		tc->velocity.y = 0;
	}

private:
	enum class State { patrol, chase, dying };
	State state;
	TransformComponent* tc;
	TransformComponent* playerTransform;
	Entity* player;

	float aggro = 250.0f;
	bool hasCivilian = false;
	bool movingLeft = true;
	int startX, endX;
	int speed;
};