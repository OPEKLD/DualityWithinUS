#pragma once

#include "ECS.h"
#include "Components.h"
#include "TimeManager.h"
#include <iostream>

class HealthComponent : public Component
{
public:
	int hp, maxHp;
	Uint32 cooldown;
	TimeManager* tm;
	bool isCoolingDown = false;

	HealthComponent(int pHp, int pMaxHp, TimeManager* tm)
	{
		hp = pHp;
		maxHp = pMaxHp;
		this->tm = tm;
	}

	void takeDamage(int damage, Uint32 coolDown)
	{
		if (!isCoolingDown)
		{
			hp -= damage;
			std::cout << "Remaining HP: " << hp << "/ " << maxHp << std::endl;
			isCoolingDown = true;
			cooldown = coolDown;
			entity->getComponent<SpriteComponent>().setTransparency(128);
			if (cooldown > 0)
			{
				tm->AddTimer(cooldown, [this]() {
					this->removeCoolDown();
				});
			}
		}
	}

	void takeDamage(int damage)
	{
		std::cout << "Correct damage function" << std::endl;
		hp -= damage;
		std::cout << "Remaining HP: " << hp << "/ " << maxHp << std::endl;
	}

	void restoreHp(int restoreAmt)
	{
		hp += restoreAmt;
		if (hp > maxHp)
		{
			hp = maxHp;
		}
	}

	void removeCoolDown()
	{
		isCoolingDown = false;
		entity->getComponent<SpriteComponent>().setTransparency(255);
	}
};