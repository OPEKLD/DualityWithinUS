#pragma once

#include "Game.h"
#include "ECS.h"
#include "inyouComponent.h"
#include "HomingAttackComponent.h"
#include "Components.h"

class KeyboardController : public Component
{
public:
	TransformComponent *transform;
	SpriteComponent* sprite;
	bool firePressed = false;
	bool hasMedPack = false;

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
	}

	void update() override
	{
		if (Game::event.type == SDL_KEYDOWN)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = -1;
				break;
			case SDLK_a:
				transform->velocity.x = -1;
				break;
			case SDLK_s:
				transform->velocity.y = 1;
				break;
			case SDLK_d:
				transform->velocity.x = 1;
				break;
			case SDLK_q:
				if (hasMedPack)
				{
					std::cout << "Throwing medicine" << std::endl;
					hasMedPack = false;
					entity->getComponent<HealthComponent>().restoreHp(15);
					if (entity->hasComponent<InyouComponent>())
					{
						entity->getComponent<InyouComponent>().scaleUpdate(2);
						if (entity->getComponent<InyouComponent>().currentChar == 0)
						{
							sprite->swap("Assets/Player/AngelicHeroine.png");
						}
						else
						{
							sprite->swap("Assets/Player/DevilishPrincess.png");
						}
						entity->getComponent<InyouComponent>().carryingMedPack = false;
					}
				}
				break;
			case SDLK_r:
				std::cout << entity->getComponent<TransformComponent>().position.x << ", " << entity->getComponent<TransformComponent>().position.y << std::endl;
				break;
			case SDLK_SPACE:
				if (!hasMedPack)
				{
					firePressed = true;
				}
				break;
			case SDLK_o:
				if (entity->hasComponent<HomingAttackComponent>())
				{
					entity->getComponent<HomingAttackComponent>().attack();
				}
			}
		}

		if (Game::event.type == SDL_KEYUP)
		{
			switch (Game::event.key.keysym.sym)
			{
			case SDLK_w:
				transform->velocity.y = 0;
				break;
			case SDLK_a:
				transform->velocity.x = 0;
				break;
			case SDLK_s:
				transform->velocity.y = 0;
				break;
			case SDLK_d:
				transform->velocity.x = 0;
				break;
			}
		}
	}
};