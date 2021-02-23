#pragma once

#include "../ActionsComponent/ActionsComponent.h"

class IABase;
class IAComponent;

class IABase
{
public:
	IAComponent* IAComponent;

	IABase() {}
	virtual ~IABase() {}

	void init(Entity* owner, std::vector<Entity*> targets)
	{
		this->owner = owner;
		this->targets = targets;

		focusedTarget = targets[0];

		transform = &owner->getComponent<TransformComponent>();
		actions = &owner->getComponent<ActionsComponent>();
	}

	virtual void update() {}

	void addTarget(Entity* newTarget, bool focus = false)
	{
		targets.emplace_back(newTarget);

		if (focus) focusedTarget = targets[targets.size() - 1];
	}

	void changeTargets(std::vector<Entity*> newTargets)
	{
		targets = newTargets;
		focusedTarget = targets[0];
	}

	void changeTarget(Entity* newTarget)
	{
		targets.clear();
		targets.emplace_back(newTarget);
		focusedTarget = newTarget;
	}

	Vector2D getDistanceToTarget();

protected:
	Entity* owner = nullptr;
	Entity* focusedTarget = nullptr;
	std::vector<Entity*> targets;

	TransformComponent* transform;
	ActionsComponent* actions;
};

class IAComponent : public Component
{
public:
	IAComponent() {}
	IAComponent(Entity* target) { targets.emplace_back(target); }

	IAComponent(std::vector<Entity*> targets) : targets(targets) {}
	~IAComponent() {}

	void init() {}
	
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

		newIA->init(entity, targets);
	}

	template<typename T> T& getIA() const
	{
		return *static_cast<T*>(IA);
	}

private:
	IABase* IA;

	std::vector<Entity*> targets;
};
