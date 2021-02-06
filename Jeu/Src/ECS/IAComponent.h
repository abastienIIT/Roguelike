#pragma once

#include "Components.h"

class IAComponent : public Component
{
public:
	IAComponent() {}
	IAComponent(Entity* target, int type) : target(target), type(type) {}
	~IAComponent() {}

	void init() 
	{
		transform = &entity->getComponent<TransformComponent>();
		actions = &entity->getComponent<ActionsComponent>();

		lastShot = SDL_GetTicks();
	}
	void update() 
	{
		if (type == 1)
		{
			simpleFollowFar();
			simpleShoot();
		}
		else if (type == 2)
		{
			simpleFollow();
		}
	}

	void simpleFollow();
	void simpleFollowFar();
	void simpleShoot();

private:
	TransformComponent* transform;
	ActionsComponent* actions;

	int type;

	Entity* target;

	Uint32 lastShot;
};