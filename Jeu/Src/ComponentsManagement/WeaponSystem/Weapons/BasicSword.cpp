#include "BasicSword.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"
#include "../../../Collisions/Collision.h"

void BasicSword::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	WeaponBase::init(owner,targets,slot);

	owner->getComponent<SpriteComponent>().setAnimatedAsset("BasicSword", slot);

	transform = &owner->getComponent<TransformComponent>();
	ownerPos = &transform->position;

	drawAllColliders = Globalbilboulga::getInstance()->getDrawAllColliders();
}

void BasicSword::update()
{
	drawHB = false;

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
			else
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


void BasicSword::attackPressed()
{
	if (!attacking)
	{
		if (SDL_GetTicks() - lastAttack < 500 && lastAttack2 == false)
		{
			attack2 = true;
		}

		owner->getComponent<TransformComponent>().speed /= 2;
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
		owner->getComponent<ActionsComponent>().canMove = false;
		owner->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
		attackStart = SDL_GetTicks();
		attackRealeaseDone = true;
	}
}

void BasicSword::attackSpecialPressed()
{
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

void BasicSword::checkHit()
{
	SDL_Rect hitbox;
	
	if (!attack2)
	{
		if (transform->facingRight) hitbox.x = ownerPos->x + (transform->width / 2 + hitboxAttack1.x) * transform->scale;
		else hitbox.x = ownerPos->x - (-transform->width / 2 + hitboxAttack1.x + hitboxAttack1.w) * transform->scale;

		hitbox.y = ownerPos->y + hitboxAttack1.y * transform->scale;
		hitbox.w = hitboxAttack1.w * transform->scale;
		hitbox.h = hitboxAttack1.h * transform->scale;
	}
	else
	{
		if (transform->facingRight) hitbox.x = ownerPos->x + (transform->width / 2 + hitboxAttack2.x) * transform->scale;
		else hitbox.x = ownerPos->x - (-transform->width / 2 + hitboxAttack2.x + hitboxAttack2.w) * transform->scale;

		hitbox.y = ownerPos->y + hitboxAttack2.y * transform->scale;
		hitbox.w = hitboxAttack2.w * transform->scale;
		hitbox.h = hitboxAttack2.h * transform->scale;
	}

	for (Entity* t : *targets)
	{
		if (std::find(targetsHit.begin(), targetsHit.end(), t) != targetsHit.end()) continue;

		if (Collision::AABB(t->getComponent<ColliderComponent>().collider, hitbox))
		{
			t->getComponent<RessourcesComponent>().takeDamage(damages);
			targetsHit.emplace_back(t);
		}
	}

	hitboxToDraw = hitbox;
	if (*drawAllColliders) drawHB = true;
}

void BasicSword::drawHitbox()
{
	if (!drawHB) return;

	SDL_Rect border;

	border.x = hitboxToDraw.x - Globalbilboulga::getInstance()->getCamera()->x;
	border.y = hitboxToDraw.y - Globalbilboulga::getInstance()->getCamera()->y;
	border.w = hitboxToDraw.w;
	border.h = hitboxToDraw.h;

	TextureManager::DrawRectangle(&border);
}
