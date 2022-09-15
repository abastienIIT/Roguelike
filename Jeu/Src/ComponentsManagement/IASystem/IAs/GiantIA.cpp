#include "GiantIA.h"
#include "SDL2/SDL.h"
#include "../../WeaponSystem/WeaponSystem.h"

void GiantIA::init(Entity* owner, std::vector<Entity*> targets)
{
	IABase::init(owner, targets);
	lastTime = SDL_GetTicks();
}

void GiantIA::update()
{
	Vector2D dist = getDistanceToTarget();
	
	if (dist.y > 150 || dist.y < -350) return;

	if ((dist.x < 0 && transform->horizontalFlip == true) ||
		(dist.x > 0 && transform->horizontalFlip == false))
		actions->turnAround();

	if (dist.x > 150 && dist.x < 500)
		actions->walk(-1);
	else if (dist.x < -150 && dist.x > -500)
		actions->walk(1);
	else
	{
		if (SDL_GetTicks() - lastTime > 2000)
		{
			if (dist.x > -50 && dist.x < 50)
				owner->getComponent<WeaponComponent>().getWeapon(false)->attackPressed(1);
			else
				owner->getComponent<WeaponComponent>().getWeapon(false)->attackPressed(0);
			
			lastTime = SDL_GetTicks();
		}
	}
}
