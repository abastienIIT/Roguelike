#include "BigKnightIA.h"
#include "SDL2/SDL.h"
#include "../../WeaponSystem/WeaponSystem.h"

void BigKnightIA::init(Entity* owner, std::vector<Entity*> targets)
{
	IABase::init(owner, targets);
	lastTime = SDL_GetTicks();
}

void BigKnightIA::update()
{
	int dist = getDistanceToTarget().x;
	if (dist > 150 || (dist > -100 && dist < 0))
		actions->walk(-1);
	else if ((dist < 100 && dist > 0) || dist < -150)
		actions->walk(1);
	else
	{
		if (SDL_GetTicks() - lastTime > 2000 && !attackDone)
		{
			owner->getComponent<WeaponComponent>().getWeapon(false)->attackPressed(1);
			attackDone = true;
			//lastTime = SDL_GetTicks();
		}
		else if (SDL_GetTicks() - lastTime > 4000)
		{
			owner->getComponent<WeaponComponent>().getWeapon(false)->attackPressed(0);
			lastTime = SDL_GetTicks();
			attackDone = false;
		}
	}



}
