#include "CivilianSpawner.h"

CivilianSpawner::CivilianSpawner(Manager* mgr, Collision* cls, TimeManager* tm, Entity* player)
{
	this->mgr = mgr;
	this->cls = cls;
	this->tm = tm;
	this->player = player;

	civilianData["injured"] = {
		Vector2D(1680, 715),
		Vector2D(1470, 37),
		Vector2D(2995, 665),
		Vector2D(3715, 570),
		Vector2D(5360, 765)
	};
	civilianData["healthy"] = {
		Vector2D(260, 615),
		Vector2D(2045, 515),
		Vector2D(3100, 135),
		Vector2D(4245, 565),
		Vector2D(2595, 330)
	};
}

void CivilianSpawner::update()
{
	for (auto& c : civilians)
	{
		cls->checkCollisions(*c);
	}
}

void CivilianSpawner::spawn()
{
	if (civilianData.find("injured") != civilianData.end())
	{
		for (const auto& pos : civilianData["injured"])
		{
			auto& civilian(mgr->addEntity());

			civilian.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
			civilian.addComponent<SpriteComponent>("Assets/Civilians/InjuredCivilianPink .png");
			civilian.addComponent<HealthComponent>(5, 5, tm);
			civilian.addComponent<ColliderComponent>("wounded");
			civilian.addComponent<SquealComponent>(player);
			civilians.push_back(&civilian);
		}
	}

	if (civilianData.find("healthy") != civilianData.end())
	{
		for (const auto& pos : civilianData["healthy"])
		{
			auto& civilian(mgr->addEntity());

			civilian.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
			civilian.addComponent<SpriteComponent>("Assets/Civilians/CivilianPinkCasual.png");
			civilian.addComponent<HealthComponent>(5, 5, tm);
			civilian.addComponent<ColliderComponent>("casual");
			civilians.push_back(&civilian);
		}
	}
}
