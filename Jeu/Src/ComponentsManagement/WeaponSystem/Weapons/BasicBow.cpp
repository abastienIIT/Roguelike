#include "BasicBow.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"
#include "../../../ProjectileCreator.h"
#include "../../ProjectileSystem/Projectiles/Arrow.h"

void BasicBow::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	WeaponBase::init(owner, targets, slot);

	Globalbilboulga::getInstance()->getAssetManager()->addAnimatedTexture("BasicBow", "assets/Player/Weapons/BasicBow/BasicBow.png", "assets/Player/Weapons/BasicBow/Infos.txt");

	owner->getComponent<SpriteComponent>().setTex("BasicBow", slot);
	animations = owner->getComponent<SpriteComponent>().animations[slot];
}

void BasicBow::update()
{
	if (attacking)
	{
		if (attackHold)
		{
			if (SDL_GetTicks() - attackStart < 200) owner->getComponent<SpriteComponent>().play("Draw");
			else owner->getComponent<SpriteComponent>().play("Ready");
		}
		else
		{
			if (SDL_GetTicks() - attackStart < 100)
			{
				owner->getComponent<SpriteComponent>().play("Release");		
			}
			else
			{
				owner->getComponent<SpriteComponent>().playDefault();
				owner->getComponent<SpriteComponent>().update();
				attacking = false;
				attackRealeaseDone = false;
				owner->getComponent<ActionsComponent>().attacking = false;
			}
		}
	}
}

void BasicBow::attackPressed()
{
	if (!attacking)
	{
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
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
		if (projStrength > 300) projStrength = 300;

		attackHold = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;

		Vector2D projVelo;
		Vector2D startPos = owner->getComponent<TransformComponent>().position;
		int ownerScale = owner->getComponent<TransformComponent>().scale;
		bool spriteFlipped = false;
		if (owner->getComponent<TransformComponent>().facingRight)
		{
			projVelo.x = 2;
			startPos.x += 9 * ownerScale;
			startPos.y += 12 * ownerScale;
		}
		else
		{
			projVelo.x = -2;
			startPos.x -= 9 * ownerScale;
			startPos.y += 12 * ownerScale;
			spriteFlipped = true;
		}

		Globalbilboulga::getInstance()->getProjectileCreator()->createProjectile<Arrow>(startPos, targets, projVelo, projStrength, spriteFlipped);
	}
}
