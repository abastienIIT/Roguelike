#include "SimpleFollowFarAndShoot.h"
#include "../../ProjectileSystem/Projectiles.h"
#include "../../../Game.h"
#include "../../../Common/Globalbilboulga.h"
#include "../../../ProjectileCreator.h"
#include "../../ActionsComponent/ActionsComponent.h"


void SimpleFollowFarAndShoot::update()
{
	Vector2D entityPos = transform->position;
	Vector2D targetPos = focusedTarget->getComponent<TransformComponent>().position;
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
	DoubleVector projVelo;

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
		if (actions->canShoot())
		{
			Globalbilboulga::getInstance()->getProjectileCreator()->createProjectile<Fireball>(*startPos, &targets, projVelo);
		}
		lastShot = SDL_GetTicks();
	}

	Vector2D dist = IABase::getDistanceToTarget();
}

