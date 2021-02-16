#pragma once

#include "../IAs.h"

class SimpleFollowFarAndShoot : public IABase
{
public:
	SimpleFollowFarAndShoot() 
	{
		lastShot = SDL_GetTicks();
	}

	void update();

private:
	Uint32 lastShot;
};
