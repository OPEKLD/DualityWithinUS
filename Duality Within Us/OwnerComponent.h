#pragma once

#include "ECS.h"
#include "Components.h"

class OwnerComponent : public Component 
{
public:
	Entity* owner;
	OwnerComponent() = default;
	OwnerComponent(Entity* o) : owner(o) {}
	void removeOwner()
	{
		if (owner)
		{
			owner = nullptr;
		}
	}
};