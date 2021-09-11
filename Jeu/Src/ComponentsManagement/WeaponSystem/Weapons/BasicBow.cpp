#include "BasicBow.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"
#include "../../../ProjectileCreator.h"
#include "../../ProjectileSystem/Projectiles/Arrow.h"

void BasicBow::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	WeaponBase::init(owner, targets, slot);

	owner->getComponent<SpriteComponent>().setAnimatedAsset("BasicBow", slot);
}

void BasicBow::update()
{
	if (attacking)
	{
		if (attackHold)
		{
			if (SDL_GetTicks() - attackStart < 200) owner->getComponent<SpriteComponent>().play("Draw",1);
			else owner->getComponent<SpriteComponent>().play("Ready",1);
		}
		else
		{
			if (SDL_GetTicks() - attackStart < 100)
			{
				owner->getComponent<SpriteComponent>().play("Release",1);		
			}
			else
			{
				owner->getComponent<SpriteComponent>().setCurrentTexture(0);
				owner->getComponent<SpriteComponent>().update();
				attacking = false;
				attackRealeaseDone = false;
				owner->getComponent<ActionsComponent>().attacking = false;
				owner->getComponent<TransformComponent>().speed *= 2;
			}
		}
	}
}

void BasicBow::attackPressed()
{
	if (!attacking)
	{
		owner->getComponent<TransformComponent>().speed /= 2;
		owner->getComponent<SpriteComponent>().setCurrentTexture(slot);
		attacking = true;
		attackHold = true;
		attackStart = SDL_GetTicks();
	}
}

void BasicBow::attackRealeased()
{
	if (!attackRealeaseDone && attacking)
	{
		int projStrength = SDL_GetTicks() - attackStart;
		if (projStrength > 500) projStrength = 500;
		double powerCoef = (double)projStrength / 500.0;

		attackHold = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;

		DoubleVector projVelo;
		Vector2D startPos = owner->getComponent<TransformComponent>().position;
		int ownerScale = owner->getComponent<TransformComponent>().scale;
		bool spriteFlipped = false;
		if (owner->getComponent<TransformComponent>().facingRight)
		{
			projVelo.x = 9 + 8 * powerCoef;
			startPos.x += 9 * ownerScale;
			startPos.y += 12 * ownerScale;
		}
		else
		{
			projVelo.x = -9 - 8 * powerCoef;
			startPos.x += 9 * ownerScale;
			startPos.y += 12 * ownerScale;
			spriteFlipped = true;
		}

		Globalbilboulga::getInstance()->getProjectileCreator()->createProjectile<Arrow>(startPos, targets, projVelo, projStrength, spriteFlipped, arrowDamage);
	}
}

void BasicBow::attackInterrupt()
{
	if (!attacking) return;

	owner->getComponent<TransformComponent>().speed *= 2;
	owner->getComponent<SpriteComponent>().setCurrentTexture(0);
	owner->getComponent<SpriteComponent>().update();
	owner->getComponent<ActionsComponent>().attacking = false;

	attackRealeaseDone = false;
	attacking = false;
	attackHold = false;
}
