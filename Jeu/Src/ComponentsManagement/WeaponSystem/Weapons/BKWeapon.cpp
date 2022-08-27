#include "BKWeapon.h"
#include "../../Components.h"

void BKWeapon::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	damages = 1;
	knockback = 7;

	CacWeapon::init(owner, targets, slot);
}

void BKWeapon::update()
{
	if (!attacking) return;

	if (SDL_GetTicks() - attackStart < 400)
	{
		owner->getComponent<SpriteComponent>().play(currentAttack);

		if (SDL_GetTicks() - attackStart > 300) checkHit();
	}

	else
	{
		owner->getComponent<SpriteComponent>().update();
		attacking = false;

		owner->getComponent<ActionsComponent>().attacking = false;
		owner->getComponent<ActionsComponent>().canMove = true;
		targetsHit.clear();
	}
}

void BKWeapon::attackPressed(int attackID)
{
	if (attacking) return;

	owner->getComponent<SpriteComponent>().setCurrentTexture(0);
	attacking = true;
	attackStart = SDL_GetTicks();

	switch(attackID)
	{
	case 0: 
		currentAttack = "AttackAbove";
		hitboxDimensions = AttackAboveHitbox;
		break;

	case 1: 
		currentAttack = "AttackSide";
		hitboxDimensions = AttackSideHitbox;
		break;

	default: 
		attacking = false;
	}
}

void BKWeapon::attackRealeased(int attackID)
{
}

void BKWeapon::attackInterrupt()
{
}
