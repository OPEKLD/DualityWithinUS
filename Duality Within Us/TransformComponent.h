#pragma once

#include "Components.h"
#include "Vector2D.h"


class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	int height, width, scale;

	int speed = 5;

	TransformComponent()
	{
		position.Zero();
	}

	TransformComponent(float px, float py, int w, int h, int s)
	{
		position.x = px;
		position.y = py;
		width = w;
		height = h;
		scale = s;
	}

	void init() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	void update() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};