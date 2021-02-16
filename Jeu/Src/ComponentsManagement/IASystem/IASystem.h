#pragma once

#include "../../ComponentsManagement/Components.h"

class IABase;
class IAComponent;

class IABase
{
public:
	IAComponent* IAComponent;

	IABase() {}

	void init(Entity* owner, Entity* target)
	{
		this->owner = owner;
		this->target = target;

		transform = &owner->getComponent<TransformComponent>();
		actions = &owner->getComponent<ActionsComponent>();
	}

	virtual void update() {}

	virtual ~IABase() {}

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
	}
	void update()
	{
		IA->update();
	}

	template <typename T, typename... TArgs>
	void setIA(TArgs&&... mArgs)
	{
		T* newIA(new T(std::forward<TArgs>(mArgs)...));
		newIA->IAComponent = this;
		IA = newIA;

		newIA->init(entity, target);
	}

	template<typename T> T& getIA() const
	{
		return *static_cast<T*>(IA);
	}

private:
	IABase* IA;

	TransformComponent* transform;
	ActionsComponent* actions;

	Entity* target;
};
