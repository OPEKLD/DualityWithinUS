#include "MedPackSpawner.h"

MedPackSpawner::MedPackSpawner(Manager* mgr, Collision* cls)
{
	this->mgr = mgr;
	this->cls = cls;
	medPackData = {
		Vector2D(1020, 805),
		Vector2D(2055, 220),
		Vector2D(3915, 785),
		Vector2D(4660, 820),
		Vector2D(3585, 15)
	};
}

void MedPackSpawner::update()
{
	for (auto& m : medPacks)
	{
		cls->checkCollisions(*m);
	}
}

void MedPackSpawner::spawn()
{
	for (auto& m : medPackData)
	{
		auto& medpack(mgr->addEntity());
		medpack.addComponent<TransformComponent>(m.x, m.y, 32, 32, 2);
		medpack.addComponent<SpriteComponent>("Assets/Items/MedicineGlass.png");
		medpack.addComponent<ColliderComponent>("medpack");

		medPacks.push_back(&medpack);
	}
}
