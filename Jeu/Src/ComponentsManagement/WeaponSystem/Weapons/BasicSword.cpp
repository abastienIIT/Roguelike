#include "BasicSword.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"

void BasicSword::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	WeaponBase::init(owner,targets,slot);

	Globalbilboulga::getInstance()->getAssetManager()->addAnimatedTexture("BasicSword", "assets/Player/Weapons/BasicSword/BasicSword.png", "assets/Player/Weapons/BasicSword/Infos.txt");
	
	owner->getComponent<SpriteComponent>().setTex("BasicSword", slot);
	owner->getComponent<SpriteComponent>().newsetTex("BasicSword",0, slot);
	animations = owner->getComponent<SpriteComponent>().animations[slot];
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
			if (SDL_GetTicks() - attackStart < 300)
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
				owner->getComponent<SpriteComponent>().playDefault();
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
		owner->getComponent<SpriteComponent>().doubleSize = true;
		attacking = true;
		attackHold = true;
		attackStart = SDL_GetTicks();
	}
}

void BasicSword::attackRealeased()
{
	if (!attackRealeaseDone && attacking)
	{
		owner->getComponent<SpriteComponent>().animLoop = false;
		attackHold = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;
	}
}

void BasicSword::attackSpecialPressed()
{
}
