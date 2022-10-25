#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

class Component;
class Entity;
class Manager;

using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
	static ComponentID lastID = 0u;
	return lastID++;
}

template <typename T> inline ComponentID getComponentTypeID() noexcept
{
	static ComponentID typeID = getNewComponentTypeID();
	return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitSet = std::bitset<maxGroups>;
using ComponentArray = std::array<Component*, maxComponents>;

class Component
{
public:
	Entity* entity = nullptr;

	virtual void init() {}
	virtual void update() {}
	virtual void draw() {}

	virtual ~Component() {}
};

class Entity
{
private:
	Manager& manager;
	bool active = false;
	bool toBeDestroyed = false;
	std::vector<std::unique_ptr<Component>> components;

	ComponentArray componentArray = { nullptr };
	ComponentBitSet componentBitSet;
	GroupBitSet groupBitSet;

public:
	Entity(Manager& mManager) : manager(mManager) {}

	void update()
	{
		for (auto& c : components) c->update();
	}

	void draw() 
	{
		for (auto& c : components) c->draw();
	}
	
	bool isActive() const { return active; }
	void deactivate() { active = false; }
	void activate() { active = true; }

	bool isToBeDestroyed() const { return toBeDestroyed; }
	void destroy() { toBeDestroyed = true; }

	bool hasGroup(Group mGroup)
	{
		return groupBitSet[mGroup];
	}

	void setGroup(Group mGroup)
	{
		groupBitSet[mGroup] = true;
	}

	void addGroup(Group mGroup);

	void delGroup(Group mGroup)
	{
		groupBitSet[mGroup] = false;
	}

	template <typename T> bool hasComponent() const
	{
		return componentBitSet[getComponentTypeID<T>()];
	}

	template <typename T, typename... TArgs>
	T& addComponent(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->entity = this;
		std::unique_ptr<Component> uPtr{ c };
		components.emplace_back(std::move(uPtr));

		componentArray[getComponentTypeID<T>()] = c;
		componentBitSet[getComponentTypeID<T>()] = true;

		c->init();
		return *c;
	}

	template<typename T> T& getComponent() const
	{
		auto ptr(componentArray[getComponentTypeID<T>()]);
		return *static_cast<T*>(ptr);
	}
};

class Manager
{
private:
	std::vector<std::unique_ptr<Entity>> allEntities;
	std::vector<Entity*> entitiesActivated;
	std::array<std::vector<Entity*>, maxGroups> groupedEntities;
	std::vector<Entity*> entitiesToDestroy;

	size_t projectilesGroup = 0;
	size_t enemiesGroup = 0;

public:
	void update()
	{
		for (auto& e : entitiesActivated) e->update();
	}

	void draw()
	{
		for (auto& e : entitiesActivated) e->draw();
	}

	void drawGroup(Group mGroup)
	{
		for (auto& e : getGroup(mGroup)) e->draw();
	}

	void refresh()
	{
		for (Entity* e : groupedEntities[projectilesGroup])
		{
			if (e->isToBeDestroyed())
			{
				groupedEntities[projectilesGroup].erase(std::remove(groupedEntities[projectilesGroup].begin(), groupedEntities[projectilesGroup].end(), e), groupedEntities[projectilesGroup].end());
				entitiesActivated.erase(std::remove(entitiesActivated.begin(), entitiesActivated.end(), e), entitiesActivated.end());
			}
		}

		for (Entity* e : groupedEntities[enemiesGroup])
		{
			if (e->isToBeDestroyed())
			{
				groupedEntities[enemiesGroup].erase(std::remove(groupedEntities[enemiesGroup].begin(), groupedEntities[enemiesGroup].end(), e), groupedEntities[enemiesGroup].end());
				entitiesActivated.erase(std::remove(entitiesActivated.begin(), entitiesActivated.end(), e), entitiesActivated.end());
			}
		}
		
		//std::cout << groupedEntities[projectilesGroup].size() << std::endl;
		//std::cout << entitiesActivated.size() << std::endl;
		//std::cout << allEntities.size() << std::endl;
		/*
		for (auto i(0u); i < maxGroups; i++)
		{
			auto& v(groupedEntities[i]);
			v.erase(
				std::remove_if(std::begin(v), std::end(v),
					[i](Entity* mEntity)
					{
						return mEntity->isToBeDestroyed() || !mEntity->hasGroup(i);
					}),
				std::end(v));
		}
		
		entitiesActivated.erase(std::remove_if(std::begin(entitiesActivated), std::end(entitiesActivated),
			[](const Entity* mEntity)
			{
				return mEntity->isToBeDestroyed();
			}),
				std::end(entitiesActivated));
				*/
	}

	void addToGroup(Entity* mEntity, Group mGroup)
	{
		groupedEntities[mGroup].emplace_back(mEntity);
	}

	void setGroup(std::vector<Entity*>* newGroup, Group mGroup)
	{
		groupedEntities[mGroup] = *newGroup;
	}

	void clearGroup(Group mGroup)
	{
		groupedEntities[mGroup].clear();
	}

	std::vector<Entity*>& getGroup(Group mGroup)
	{
		return groupedEntities[mGroup];
	}

	void clearActivatedEntities()
	{
		for (Entity* e : entitiesActivated)
			deactivateEntity(e);

		entitiesActivated.clear();
	}

	void activateEntity(Entity* entity)
	{
		entity->activate();
		entitiesActivated.emplace_back(entity);
	}

	void activateManyEntities(std::vector<Entity*>* entities)
	{
		for (Entity* e : *entities)
			activateEntity(e);
	}

	void deactivateEntity(Entity* entity)
	{
		entity->deactivate();
	}

	void deactivateManyEntities(std::vector<Entity*>* entities)
	{
		for (Entity* e : *entities)
			deactivateEntity(e);
	}

	Entity& addEntity(bool activate = true)
	{
		Entity* e = new Entity(*this);
		if (activate)
		{
			e->activate();
			std::unique_ptr<Entity> uPtr{ e };
			allEntities.emplace_back(std::move(uPtr));
			entitiesActivated.emplace_back(e);
		}
		else 
		{
			std::unique_ptr<Entity> uPtr{ e };
			allEntities.emplace_back(std::move(uPtr));
		}
		return *e;
	}

	void setProjectilesGroup(size_t group) { projectilesGroup = group; }
	void setEnemiesGroup(size_t group) { enemiesGroup = group; }
};

