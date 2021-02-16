#pragma once

#include "../../ComponentsManagement/Components.h"

class IAs;
class IAComponent;

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
		//IA = *static_cast<T*>(i);

		i->init(entity, target);
		return *i;
	}

	template<typename T> T& getIA() const
	{
		auto ptr(IA);
		return *static_cast<T*>(ptr);
	}

private:
	std::unique_ptr<IAs> IA;

	TransformComponent* transform;
	ActionsComponent* actions;

	Entity* target;

	Uint32 lastShot;
};
