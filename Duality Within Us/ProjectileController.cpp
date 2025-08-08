#include "ProjectileController.h"

ProjectileController::ProjectileController(Manager* mgr, Entity* plr, TimeManager* tm)
{
	manager = mgr;
	player = plr;
	this->tm = tm;
	kc = &player->getComponent<KeyboardController>();
}

void ProjectileController::update()
{
	if (kc->firePressed)
	{
		int direction = player->getComponent<SpriteComponent>().getDirection();
		Vector2D plrPos = player->getComponent<TransformComponent>().position;
		Spawn(plrPos, direction);
		kc->firePressed = false;
	}
}

void ProjectileController::Spawn(Vector2D pos, int direction)
{
	int id;
	path = (player->getComponent<InyouComponent>().currentChar == 0) ? "Assets/Projectiles/ProjectileA.png" : "Assets/Projectiles/ProjectileD.png";
	float xPos = pos.x + (float)direction;
	auto& projectile(manager->addEntity());
	projectile.addComponent<OwnerComponent>(player);
	projectile.addComponent<TransformComponent>(xPos, pos.y + 30, 16, 28, 2);
	projectile.addComponent<SpriteComponent>(path);
	projectile.addComponent<RangedAttack>(direction);
	projectile.addComponent<ColliderComponent>("pBullet");
	tm->AddTimer(5000, [this]() {
		this->DestroyProjectile();
	});
	projectiles.push_back(&projectile);
} 

void ProjectileController::DestroyProjectile()
{
	if (!projectiles.empty())
	{
		Entity* e = projectiles.front();

		projectiles.erase(projectiles.begin());

		if (e != nullptr)
		{
			e->getComponent<OwnerComponent>().removeOwner();
			e->destroy();
		}
	}
}
