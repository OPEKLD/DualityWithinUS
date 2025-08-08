#include "Collisions.h"

Collision::Collision(Manager* mgr, TimeManager* tm)
{
	this->mgr = mgr;
	this->tm = tm;
	CollisionMatrix[std::make_pair("player", "enemy")] = PlayerEnemyCollision;
	CollisionMatrix[std::make_pair("enemy", "pBullet")] = EnemyProjectileCollision;
	CollisionMatrix[std::make_pair("player", "wounded")] = PlayerWoundedCollision;
	CollisionMatrix[std::make_pair("player", "medpack")] = PlayerMedpackCollision;
	CollisionMatrix[std::make_pair("wounded", "pBullet")] = CivilianProjectileCollision;
	CollisionMatrix[std::make_pair("casual", "pBullet")] = CivilianProjectileCollision;
}

// Axis-Aligned Bounding Box collision
bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}
	return false;
}

void Collision::checkCollisions(Entity& en)
{
	if (!en.hasComponent<ColliderComponent>())
	{
		return;
	}

	auto& mainCol = en.getComponent<ColliderComponent>();

	for (auto& e : mgr->getEntities())
	{
		if (&en == e.get()) continue;

		if (e->hasComponent<ColliderComponent>())
		{
			auto& otherCollider = e->getComponent<ColliderComponent>();

			if (en.isActive() == true)
			{
				if (AABB(mainCol.collider, otherCollider.collider))
				{
					//std::cout << "Collision detected" << std::endl;
					std::string tagA = mainCol.tag;
					std::string tagB = otherCollider.tag;

					auto it = CollisionMatrix.find({ tagA, tagB });
					if (it != CollisionMatrix.end())
					{
						CollisionFunction handler = it->second;
						handler(&en, e.get());
					}
				}
			}
		}
	}
}




// COLLISION MATRIX FUNCTIONS

void Collision::PlayerEnemyCollision(Entity* player, Entity* enemy)
{
	player->getComponent<HealthComponent>().takeDamage(10, 3000);
}

void Collision::EnemyProjectileCollision(Entity* enemy, Entity* projectile)
{
	HealthComponent& hc = enemy->getComponent<HealthComponent>();
	hc.takeDamage(25);
	if (hc.hp <= 0)
	{
		enemy->getComponent<EnemyComponent>().changeState(2);
	}
	projectile->removeComponent<SpriteComponent>();
	projectile->removeComponent<ColliderComponent>();
}

void Collision::PlayerWoundedCollision(Entity* player, Entity* civilian)
{

	if (player->getComponent<KeyboardController>().hasMedPack == true)
	{
		player->getComponent<InyouComponent>().scaleUpdate(-6);
		player->getComponent<KeyboardController>().hasMedPack = false;
		civilian->getComponent<SpriteComponent>().swap("Assets/Civilians/CivilianPinkCasual.png");
		civilian->getComponent<ColliderComponent>().tag = "saved";
		civilian->getComponent<SquealComponent>().stopAudio();
		civilian->removeComponent<SquealComponent>();
		if (player->getComponent<InyouComponent>().currentChar == 0)
		{
			player->getComponent<SpriteComponent>().swap("Assets/Player/AngelicHeroine.png");
		}
		else
		{
			player->getComponent<SpriteComponent>().swap("Assets/Player/DevilishPrincess.png");
		}
		player->getComponent<InyouComponent>().carryingMedPack = false;
	}
}

void Collision::PlayerMedpackCollision(Entity* player, Entity* medpack)
{
	std::cout << "Player picked up medpack" << std::endl;
	player->getComponent<InyouComponent>().carryingMedPack = true;
	if (player->getComponent<InyouComponent>().currentChar == 0)
	{
		player->getComponent<SpriteComponent>().swap("Assets/Carrying/AngelCarryingMedpack.png");
	}
	else
	{
		player->getComponent<SpriteComponent>().swap("Assets/Carrying/DevilCarryingMedpack.png");
	}
	player->getComponent<KeyboardController>().hasMedPack = true;
	medpack->destroy();
}

void Collision::CivilianProjectileCollision(Entity* civilian, Entity* projectile)
{
	// Timer to temporary collisions
	civilian->getComponent<HealthComponent>().takeDamage(25, 0);
	if (civilian->getComponent<HealthComponent>().hp <= 0)
	{
		if (civilian->hasComponent<SquealComponent>())
		{
			civilian->getComponent<SquealComponent>().stopAudio();
			civilian->removeComponent<SquealComponent>();
		}
		civilian->destroy();
		Entity* player = projectile->getComponent<OwnerComponent>().owner;
		player->getComponent<InyouComponent>().scaleUpdate(2);
	}
	projectile->removeComponent<SpriteComponent>();
	projectile->removeComponent<ColliderComponent>();
	/*projectile->getComponent<OwnerComponent>().removeOwner();
	projectile->destroy();*/
}

