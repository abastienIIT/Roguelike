#include "Club.h"
#include "../../Components.h"

void Club::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	damages = 1;
	knockbackBonk = 7;
	knockbackStomp = 13;

	CacWeapon::init(owner, targets, slot);
}

void Club::update()
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

void Club::attackPressed(int attackID)
{
	if (attacking) return;

	owner->getComponent<SpriteComponent>().setCurrentTexture(0);
	attacking = true;
	attackStart = SDL_GetTicks();
	owner->getComponent<ActionsComponent>().canMove = false;

	switch (attackID)
	{
	case 0:
		currentAttack = "Bonk";
		hitboxDimensions = AttackAboveHitbox;
		knockback = knockbackBonk;
		break;

	case 1:
		currentAttack = "Stomp";
		hitboxDimensions = AttackSideHitbox;
		knockback = knockbackStomp;
		break;

	default:
		attacking = false;
	}
}

void Club::attackRealeased(int attackID)
{
}

void Club::attackInterrupt()
{
}
