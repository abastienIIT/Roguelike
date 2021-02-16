#include "SimpleFollowFarAndShoot.h"

void SimpleFollowFarAndShoot::update()
{
	Vector2D entityPos = transform->position;
	Vector2D targetPos = target->getComponent<TransformComponent>().position;
	if ((targetPos.x - entityPos.x < -200 || targetPos.x - entityPos.x > 200) && (targetPos.y - entityPos.y < 100 && targetPos.y - entityPos.y > -100))
	{
		if (targetPos.x > entityPos.x)
		{
			actions->walk(1);
		}
		else
		{
			actions->walk(-1);
		}
	}
	else transform->velocity.x = 0;


	Vector2D* startPos = new Vector2D(transform->position.x + transform->height / 2, transform->position.y + transform->width / 2);
	Vector2D projVelo;

	projVelo.y = 0;
	if (targetPos.x > entityPos.x)
	{
		projVelo.x = 2;
	}
	else
	{
		projVelo.x = -2;
	}

	if (SDL_GetTicks() - lastShot > 2000)
	{
		actions->shootProjectile(*startPos, projVelo, SDL_Rect({ 0,0,32,32 }), 200, 0, "projectile");
		lastShot = SDL_GetTicks();
	}

	Vector2D dist = IABase::getDistanceToTarget();

	//std::cout << dist.x << " " << dist.y << std::endl;
}

