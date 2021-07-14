#include "BasicSword.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"

void BasicSword::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	WeaponBase::init(owner,targets,slot);

	owner->getComponent<SpriteComponent>().setAnimatedAsset("BasicSword", slot);
}

void BasicSword::update()
{
	if (attacking)
	{
		if (attackHold)
		{
			if (!attack2)
			{
				if (SDL_GetTicks() - attackStart > 500) owner->getComponent<SpriteComponent>().play("Attack1Ready");
				else owner->getComponent<SpriteComponent>().play("Attack1Start");
			}
			else
			{
				if (SDL_GetTicks() - attackStart > 500) owner->getComponent<SpriteComponent>().play("Attack2Ready");
				else owner->getComponent<SpriteComponent>().play("Attack2Start");
			}
		}
		else
		{
			if (SDL_GetTicks() - attackStart < 325)
			{
				if (!attack2)
				{
					owner->getComponent<SpriteComponent>().play("Attack1");
					lastAttack2 = false;
				}
				else
				{
					owner->getComponent<SpriteComponent>().play("Attack2");
					lastAttack2 = true;
				}
			}
			else
			{
				owner->getComponent<SpriteComponent>().setCurrentTexture(0);
				owner->getComponent<SpriteComponent>().update();
				attacking = false;
				lastAttack = SDL_GetTicks();
				attack2 = false;
				attackRealeaseDone = false;
				owner->getComponent<ActionsComponent>().attacking = false;
			}
		}
	}
}

void BasicSword::attackPressed()
{
	if (!attacking)
	{
		if (SDL_GetTicks() - lastAttack < 500 && lastAttack2 == false)
		{
			attack2 = true;
		}

		owner->getComponent<SpriteComponent>().setCurrentTexture(slot);
		attacking = true;
		attackHold = true;
		attackStart = SDL_GetTicks();
	}
}

void BasicSword::attackRealeased()
{
	if (!attackRealeaseDone && attacking)
	{
		attackHold = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;
	}
}

void BasicSword::attackSpecialPressed()
{
}
