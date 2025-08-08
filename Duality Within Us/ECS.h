#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;

using ComponentID = std::size_t;

inline ComponentID GetComponentID()
{
	static ComponentID lastID = 0;
	return lastID++;
}

template <typename T> inline ComponentID GetComponentID() noexcept
{
	static ComponentID typeID = GetComponentID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
public:
	void update()
	{
		for (auto& c : components) c->update();
	}
	void draw() 
	{
		for (auto& c : components) c->draw();
	}
	bool isActive() const { return active;  }
	void destroy() { active = false; }

	template <typename T> bool hasComponent() const
	{
		return componentBitset[GetComponentID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* newCom = new T(std::forward<TArgs>(mArgs)...);
		newCom->entity = this;
		std::unique_ptr<Component> uPtr{ newCom };
		components.emplace_back(std::move(uPtr));

		componentArray[GetComponentID<T>()] = newCom;
		componentBitset[GetComponentID<T>()] = true;

		newCom->init();
		return *newCom;
	}

	template <typename T>
	void removeComponent()
	{
		ComponentID id = GetComponentID<T>();
		if (!componentBitset[id])
		{
			return;
		}

		Component* target = componentArray[id];

		components.erase(std::remove_if(components.begin(), components.end(), [target](const std::unique_ptr<Component>& c)
		{
			return c.get() == target;
		}),
		components.end());

		componentArray[id] = nullptr;
		componentBitset[id] = false;
	}

	template <typename T> T& getComponent() const
	{
		auto ptr(componentArray[GetComponentID<T>()]);
		return *static_cast<T*>(ptr);
	}
private:
	bool active = true;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray;
	ComponentBitSet componentBitset;
};

class Manager
{
public:

	void update() 
	{
		for (auto& e : entities) e->update();
	}

	void draw()
	{
		for (auto& e : entities) e->draw();
	}

	void refresh()
	{
		entities.erase(std::remove_if(std::begin(entities), std::end(entities), [](const std::unique_ptr<Entity>& mEntity)
		{
			return !mEntity->isActive();
		}), std::end(entities));
	}

	const std::vector<std::unique_ptr<Entity>>& getEntities() 
	{
		return entities;
	}

	Entity& addEntity()
	{
		Entity* en = new Entity();
		std::unique_ptr<Entity> uPtr{ en };
		entities.emplace_back(std::move(uPtr));
		return *en;
	}

	void clearEntities()
	{
		entities.clear();
	}
private:
	std::vector<std::unique_ptr<Entity>> entities;
};