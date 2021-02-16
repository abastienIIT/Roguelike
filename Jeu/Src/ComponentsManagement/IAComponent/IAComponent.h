#pragma once

#include "../../ComponentsManagement/Components.h"

class IAs;
class IAComponent;

using IAID = std::size_t;

inline IAID getNewIATypeID()
{
	static IAID lastID = 0u;
	return lastID++;
}

template <typename T> inline IAID getIATypeID() noexcept
{
	static IAID typeID = getNewIATypeID();
	return typeID;
}

constexpr std::size_t maxIAs = 32;

using IABitSet = std::bitset<maxIAs>;
using IAArray = std::array<IAs*, maxIAs>;

class IAs
{
public:
	IAComponent* IAComponent;

	void init(Entity* owner, Entity* target)
	{
		this->owner = owner;
		this->target = target;

		transform = &owner->getComponent<TransformComponent>();
		actions = &owner->getComponent<ActionsComponent>();
	}

	virtual void update() {}

	virtual ~IAs() {}

	Vector2D getDistanceToTarget();

protected:
	Entity* owner = nullptr;
	Entity* target = nullptr;

	TransformComponent* transform;
	ActionsComponent* actions;
};

class IAComponent : public Component
{
public:
	IAComponent() {}
	IAComponent(Entity* target) : target(target) {}
	~IAComponent() {}

	void init()
	{
		transform = &entity->getComponent<TransformComponent>();
		actions = &entity->getComponent<ActionsComponent>();

		lastShot = SDL_GetTicks();
	}
	void update()
	{
		IA->update();
	}

	template <typename T, typename... TArgs>
	T& setIA(TArgs&&... mArgs)
	{
		T* i(new T(std::forward<TArgs>(mArgs)...));
		i->IAComponent = this;
		std::unique_ptr<IAs> uPtr{ i };
		IA = std::move(uPtr);

		IABitSet.reset();

		IAArray[getIATypeID<T>()] = i;
		IABitSet[getIATypeID<T>()] = true;

		i->init(entity, target);
		return *i;
	}

	template<typename T> T& getIA() const
	{
		auto ptr(IAArray[getIATypeID<T>()]);
		return *static_cast<T*>(ptr);
	}

private:
	std::unique_ptr<IAs> IA;

	IAArray IAArray;
	IABitSet IABitSet;

	TransformComponent* transform;
	ActionsComponent* actions;

	Entity* target;

	Uint32 lastShot;
};
