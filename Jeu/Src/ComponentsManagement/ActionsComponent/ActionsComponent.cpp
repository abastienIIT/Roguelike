#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Game.h"
#include "../../Collisions/Collision.h"
#include "../Unique/RessourcesComponent.h"

#define JUMP_HEIGHT 250 //pixel
#define JUMP_INITIAL_SPEED -10
#define ROLL_TIME 320
#define ROLL_END 80

ActionsComponent::ActionsComponent()
{
	ascendingPhase = false;
	canMove = true;
	rolling = false;
}

void ActionsComponent::update()
{
    transform->velocity.x = 0;
    sprite->playDefault();


	if (ascendingPhase) jumpProcess();

	if (rolling) rollProcess();


	previousPos = transform->position;
}

void ActionsComponent::walk(const int direction)
{
	if (canMove)
	{
		transform->velocity.x = direction * transform->speed;

		if (!attacking) sprite->playCommon("Walk");
		else sprite->play("Walk", 0);

		if (direction == 1)
		{
			transform->horizontalFlip = false;
		}
		else
		{
			transform->horizontalFlip = true;
		}
	}
}

void ActionsComponent::jumpProcess()
{
	// detect max jump height reached
	if (abs(startJumpY - transform->position.y) > JUMP_HEIGHT) {
		transform->position.y = startJumpY - JUMP_HEIGHT;
		collider->update();
		ascendingPhase = false;
		//std::cout << "Height accelerationPhase over" << std::endl;
	}

	// detect celing hit
	else if (!transform->onGround && previousPos.y - transform->position.y < 2) {
		ascendingPhase = false;
		//std::cout << "hit accelerationPhase over" << std::endl;
		transform->velocity.y = Globalbilboulga::GRAVITY_STRENGTH;
	}

	else
	{
		bool smooth = (double)abs(startJumpY - transform->position.y) / JUMP_HEIGHT > 0.85;
		transform->velocity.y -= Globalbilboulga::GRAVITY_STRENGTH * (smooth ? 0.9 : 1);
		//std::cout << "Ascending" << std::endl;
	}
	//std::cout << "----" << std::endl;
	//std::cout << transform->position << std::endl;
}

void ActionsComponent::jumpStop()
{
	//std::cout << "button accelerationPhase over" << std::endl;
	ascendingPhase = false;
}

void ActionsComponent::jumpStart()
{
	if (transform->onGround && !ascendingPhase) {
		ascendingPhase = true;
		transform->onGround = false;

		transform->velocity.y = JUMP_INITIAL_SPEED;
		startJumpY = transform->position.y;
	}
}

void ActionsComponent::roll()
{
	if (rolling) return;

	if (attacking) attackInterrupt();

	rolling = true;
	canMove = false;
	transform->speed *= 2;
	rollStart = SDL_GetTicks();

	sprite->animStart = rollStart;

	collider->colliderSrc.y += collider->colliderSrc.h / 2;
	collider->colliderSrc.h /= 2;

	entity->getComponent<RessourcesComponent>().intouchable = true;
}

void ActionsComponent::rollProcess()
{
	if (SDL_GetTicks() - rollStart < ROLL_TIME)
	{
		sprite->playCommon("Roll");
	}
	else if (!canGetUp())
	{
		sprite->playCommon("RollLoop");
		rollLoopTime += 1000 / Globalbilboulga::getInstance()->getFPS();
	}
	else if (SDL_GetTicks() - rollStart < ROLL_TIME + ROLL_END + static_cast<Uint32>(rollLoopTime))
	{
		sprite->playCommon("RollEnd");
	}
	else
	{
		rolling = false;
		rollLoopTime = 0;
		canMove = true;
		transform->speed /= 2;
		collider->colliderSrc.h *= 2;
		collider->colliderSrc.y -= collider->colliderSrc.h / 2;
		entity->getComponent<RessourcesComponent>().intouchable = false;
		return;
	}

	if (!transform->horizontalFlip)
	{
		transform->velocity.x = transform->speed;
	}
	else
	{
		transform->velocity.x = -transform->speed;
	}
}

bool ActionsComponent::canGetUp()
{
	SDL_Rect toFill;

	toFill.x = collider->collider.x;
	toFill.y = collider->collider.y - collider->collider.h;
	toFill.w = collider->collider.w;
	toFill.h = collider->collider.h;

	for (auto& tc : Globalbilboulga::getInstance()->getManager()->getGroup(Game::TerrainColliders))
	{
		if (Collision::AABB(toFill, tc->getComponent<ColliderComponent>().collider))
		{
			return false;
		}
	}
	
	return true;
}

void ActionsComponent::attackPressed(int attackID, bool slot2)
{
	if (!attacking && !rolling)
	{
		entity->getComponent<WeaponComponent>().getWeapon(slot2)->attackPressed(attackID);
		attacking = true;
	}
}

void ActionsComponent::attackRealeased(int attackID, bool slot2)
{
	if (!attacking) return;
	entity->getComponent<WeaponComponent>().getWeapon(slot2)->attackRealeased(attackID);
}

void ActionsComponent::attackInterrupt()
{
	entity->getComponent<WeaponComponent>().getWeapon(false)->attackInterrupt();
	entity->getComponent<WeaponComponent>().getWeapon(true)->attackInterrupt();
}
