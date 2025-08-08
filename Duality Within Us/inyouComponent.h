#pragma once

#include "ECS.h"
#include "Components.h"
#include "TimeManager.h"
#include "HomingAttackComponent.h"
#include <unordered_map>

class InyouComponent : public Component
{
public:
	int currentChar = 0; // 0 = Angel - 1 = Devil
	int angelicordevlish = 0;
	int angTime;
	int devTime;
	int currentTimer;
	bool timerTimeout = false;
	bool carryingMedPack;

	InyouComponent(TimeManager* tm)
	{
		this->tm = tm;
	}

	void init() override
	{
		sc = &entity->getComponent<SpriteComponent>();
		angTime = 90000;
		devTime = 90000;
	}

	void startProcessing()
	{
		Uint32 timeForthis = (currentChar == 0) ? angTime : devTime;

		int id = tm->AddTimer(timeForthis, [this]() {
			this->timeOut();
		});
		//std::cout << id << std::endl;
		currentTimer = id;
	}

	void scaleUpdate(int change)
	{
		angelicordevlish += change;
		std::cout << angelicordevlish << std::endl;

		// Devil leaning
		if (angelicordevlish >= 25)
		{
			angTime = 15000;
			devTime = 180000;
		}
		else if (angelicordevlish < 25 && angelicordevlish >= 15)
		{
			angTime = 30000;
			devTime = 150000;
		}
		// Neutral leaning
		else if (angelicordevlish < 15 && angelicordevlish >= 10)
		{
			angTime = 60000;
			devTime = 120000;
		}
		else if (angelicordevlish < 10 && angelicordevlish >= -9)
		{
			angTime = 90000;
			devTime = 90000;
		}
		else if (angelicordevlish <= -10 && angelicordevlish > -15)
		{
			angTime = 120000;
			devTime = 60000;
		}
		// Angelic leaning
		else if (angelicordevlish <= -15 && angelicordevlish > -25)
		{
			angTime = 150000;
			devTime = 30000;
		}
		else if (angelicordevlish <= -25)
		{
			angTime = 180000;
			devTime = 15000;
		}
	}

	void switchCharacter()
	{
		timerTimeout = false;
		if (currentChar == 0)
		{
			if (!carryingMedPack) sc->swap("Assets/Player/DevilishPrincess.png");
			else sc->swap("Assets/Carrying/DevilCarryingMedpack.png");
			entity->addComponent<HomingAttackComponent>(tm);
			currentChar = 1;
			startProcessing();
		}
		else
		{
			// Add checking for if player is carrying medpack
			if (!carryingMedPack) sc->swap("Assets/Player/AngelicHeroine.png");
			else sc->swap("Assets/Carrying/AngelCarryingMedpack.png");
			if (entity->hasComponent<HomingAttackComponent>())
			{
				entity->removeComponent<HomingAttackComponent>();
			}
			currentChar = 0;
			startProcessing();
		}
		
		// Set up attacks

	}

	void timeOut()
	{
		timerTimeout = true;
	}

private:
	TimeManager* tm;
	SpriteComponent* sc;
};