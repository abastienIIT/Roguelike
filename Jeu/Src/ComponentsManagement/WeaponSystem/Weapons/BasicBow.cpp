#include "BasicBow.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"

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
			if (SDL_GetTicks() - attackStart < 80)
			{
				owner->getComponent<SpriteComponent>().play("Release");		
			}
			else
			{
				owner->getComponent<SpriteComponent>().setCurrentTexture(0);
				owner->getComponent<SpriteComponent>().update();
				attacking = false;
				attackRealeaseDone = false;
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
		attackHold = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;
	}
}
