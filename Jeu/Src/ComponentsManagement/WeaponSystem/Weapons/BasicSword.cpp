#include "BasicSword.h"
#include "../../Components.h"

void BasicSword::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	asset = "BasicSword";
	damages = 60;
	knockback = 3;

	CacWeapon::init(owner, targets, slot);
}

void BasicSword::update()
{
	if (attacking)
	{
		if (attackHold)
		{
			if (!attack2)
			{
				if (SDL_GetTicks() - attackStart > 500) owner->getComponent<SpriteComponent>().play("Attack1Ready",1);
				else owner->getComponent<SpriteComponent>().play("Attack1Start",1);
			}
			else
			{
				if (SDL_GetTicks() - attackStart > 500) owner->getComponent<SpriteComponent>().play("Attack2Ready",1);
				else owner->getComponent<SpriteComponent>().play("Attack2Start",1);
			}
		}
		else
		{
			if (SDL_GetTicks() - attackStart < 325)
			{
				if (!attack2)
				{
					owner->getComponent<SpriteComponent>().play("Attack1",1);
					lastAttack2 = false;
				}
				else
				{
					owner->getComponent<SpriteComponent>().play("Attack2",1);
					lastAttack2 = true;
				}

				if (SDL_GetTicks() - attackStart > 200)
				{
					checkHit();
				}
			}
			else //Fin de l'attaque
			{
				owner->getComponent<SpriteComponent>().setCurrentTexture(0);
				owner->getComponent<SpriteComponent>().update();
				attacking = false;
				lastAttack = SDL_GetTicks();
				attack2 = false;
				attackRealeaseDone = false;
				owner->getComponent<ActionsComponent>().attacking = false;
				owner->getComponent<ActionsComponent>().canMove = true;
				owner->getComponent<TransformComponent>().speed *= 2;
				targetsHit.clear();
			}
		}
	}
}


void BasicSword::attackPressed(int attackID)
{
	if (!attacking)
	{
		if (SDL_GetTicks() - lastAttack < 500 && lastAttack2 == false)
		{
			attack2 = true;
			hitboxDimensions = hitboxAttack2;
		}
		else
		{
			hitboxDimensions = hitboxAttack1;
		}

		owner->getComponent<TransformComponent>().speed /= 2;
		owner->getComponent<SpriteComponent>().setCurrentTexture(slot);
		attacking = true;
		attackHold = true;
		attackStart = SDL_GetTicks();
	}
}

void BasicSword::attackRealeased(int attackID)
{
	if (!attackRealeaseDone && attacking)
	{
		attackHold = false;
		owner->getComponent<ActionsComponent>().canMove = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;
	}
}


void BasicSword::attackInterrupt()
{
	if (!attacking) return;

	owner->getComponent<SpriteComponent>().setCurrentTexture(0);
	owner->getComponent<SpriteComponent>().update();
	attacking = false;
	lastAttack = SDL_GetTicks();
	attack2 = false;
	attackRealeaseDone = false;
	attackHold = true;
	owner->getComponent<ActionsComponent>().attacking = false;
	owner->getComponent<ActionsComponent>().canMove = true;
	owner->getComponent<TransformComponent>().speed *= 2;
}
