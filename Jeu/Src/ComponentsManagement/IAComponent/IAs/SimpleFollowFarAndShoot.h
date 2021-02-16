#pragma once

#include "../IAs.h"

class SimpleFollowFarAndShoot : public IAs
{
public:
	SimpleFollowFarAndShoot() 
	{
		lastShot = SDL_GetTicks();
	}

	void init(Entity* owner, Entity* target)
	{
		IAs::init(owner, target);
	}

	void update();

	int a = 3;

private:
	Uint32 lastShot;
};
