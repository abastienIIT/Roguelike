#pragma once

#include "../ECS.h"
#include "SDL2/SDL.h"

class TrapBase;
class TrapComponent;

class TrapBase
{
public:
	TrapComponent* trapComponent = nullptr;

	TrapBase() {}
	virtual ~TrapBase() {}

	void init(Entity* owner, std::vector<Entity*> targets)
	{
		this->owner = owner;
		this->targets = targets;
	}

	virtual void update() {}

	bool damageTargetsOnTrap(SDL_Rect* trapHitbox);

protected:
	Entity* owner = nullptr;
	std::vector<Entity*> targets;
	std::vector<Entity*> targetsHit;

	int damages = 0;
	int knockback = 0;
};

class TrapComponent : public Component
{
public:
	TrapComponent() {}
	TrapComponent(Entity* target) { targets.emplace_back(target); }
	TrapComponent(std::vector<Entity*> targets) : targets(targets) {}
	~TrapComponent() {}

	void init() {}

	void update()
	{
		Trap->update();
	}

	template <typename T, typename... TArgs>
	void setTrap(TArgs&&... mArgs)
	{
		T* newTrap(new T(std::forward<TArgs>(mArgs)...));
		newTrap->trapComponent = this;
		Trap = newTrap;

		newTrap->init(entity, targets);
	}

	template<typename T> T& getTrap() const
	{
		return *static_cast<T*>(Trap);
	}

private:
	TrapBase* Trap = nullptr;

	std::vector<Entity*> targets;
};

