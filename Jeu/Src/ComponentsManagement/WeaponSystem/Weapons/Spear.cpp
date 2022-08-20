#include "Spear.h"
#include "../../Components.h"

void Spear::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	asset = "Spear";
	damages = 120;
	hitboxDimensions = { 11,1,13,3 };

	CacWeapon::init(owner, targets, slot);
}


void Spear::update()
{
	if (!attacking) return;

	if (SDL_GetTicks() - attackStart < 200)
	{
		owner->getComponent<SpriteComponent>().play("Attack", 1);

		if (SDL_GetTicks() - attackStart > 100) checkHit();
	}

	else
	{
		owner->getComponent<SpriteComponent>().setCurrentTexture(0);
		owner->getComponent<SpriteComponent>().update();
		attacking = false;
	
		owner->getComponent<ActionsComponent>().attacking = false;
		owner->getComponent<ActionsComponent>().canMove = true;
		targetsHit.clear();
	}
}

void Spear::attackPressed(int attackID)
{
	if (!attacking)
	{
		owner->getComponent<SpriteComponent>().setCurrentTexture(slot);
		attacking = true;
		attackStart = SDL_GetTicks();
	}
}

void Spear::attackRealeased(int attackID)
{
}

void Spear::attackInterrupt()
{
	if (!attacking) return;

	owner->getComponent<SpriteComponent>().setCurrentTexture(0);
	owner->getComponent<SpriteComponent>().update();
	attacking = false;
	
	owner->getComponent<ActionsComponent>().attacking = false;
	owner->getComponent<ActionsComponent>().canMove = true;
}
