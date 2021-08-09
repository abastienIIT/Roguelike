#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"
#include "../../Common/Globalbilboulga.h"

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
			sprite->spriteFlip = SDL_FLIP_NONE;
			collider->flip(0);
			transform->facingRight = true;
		}
		else
		{
			sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
			collider->flip(1);
			transform->facingRight = false;
		}
	}
}

void ActionsComponent::jumpProcess()
{
	// detect max jump height reached
	if (abs(startJumpY - transform->position.y) > JUMP_HEIGHT) {
		//transform->position.y = startJumpY - JUMP_HEIGHT;
		ascendingPhase = false;
		//std::cout << "Heiht accelerationPhase over" << std::endl;
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
	if (attacking) attackInterrupt();

	rolling = true;
	canMove = false;
	transform->speed *= 2;
	rollStart = SDL_GetTicks();

	sprite->animStart = rollStart;

	//collider->colliderSrc.y -= collider->colliderSrc.h / 2;
	//collider->colliderSrc.h /= 2;
}

void ActionsComponent::rollProcess()
{
	if (SDL_GetTicks() - rollStart < ROLL_TIME)
	{
		sprite->playCommon("Roll");
	}
	else if (SDL_GetTicks() - rollStart < ROLL_TIME + ROLL_END)
	{
		sprite->playCommon("RollEnd");
	}
	else
	{
		rolling = false;
		canMove = true;
		transform->speed /= 2;
		return;
	}

	if (transform->facingRight)
	{
		transform->velocity.x = transform->speed;
	}
	else
	{
		transform->velocity.x = -transform->speed;
	}
}

void ActionsComponent::attackPressed(bool slot2)
{
	if (!attacking)
	{
		entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(slot2).attackPressed();
		attacking = true;
	}
}

void ActionsComponent::attackRealeased(bool slot2)
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(slot2).attackRealeased();
}

void ActionsComponent::attackSpecialPressed(bool slot2)
{
	if (!attacking)
	{
		entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(slot2).attackSpecialPressed();
		attacking = true;
	}
}

void ActionsComponent::attackSpecialRealeased(bool slot2)
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(slot2).attackSpecialRealeased();
}

void ActionsComponent::attackInterrupt()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(false).attackInterrupt();
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(true).attackInterrupt();
}
