#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"
#include "../../Common/Globalbilboulga.h"

#define JUMP_HEIGHT 250 //pixel
#define JUMP_INITIAL_SPEED -10

ActionsComponent::ActionsComponent()
{
	ascendingPhase = false;
}

void ActionsComponent::update()
{
    transform->velocity.x = 0;
    sprite->play("Idle");

	jumpProcess();

	previousPos = transform->position;
}

void ActionsComponent::walk(const int direction)
{
		if (!attacking)
	{
			transform->velocity.x = direction * transform->speed;

		if (direction == 0)
		{
			sprite->play("Idle");
		}
		else
		{
			sprite->play("Walk");

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
}

void ActionsComponent::jumpProcess()
{
	std::cout << "ascendingPhase" << ascendingPhase << std::endl;
	if (ascendingPhase) {
		bool smooth = (double)abs(startJumpY - transform->position.y) / JUMP_HEIGHT > 0.85;
		transform->velocity.y -= Globalbilboulga::GRAVITY_STRENGTH * (smooth ? 0.9 : 1);
	}

	// detect max jump height reached
	if (ascendingPhase && abs(startJumpY - transform->position.y) > JUMP_HEIGHT) {
		ascendingPhase = false;
		//std::cout << "Heiht accelerationPhase over" << std::endl;
	}

	// detect celing hit
	if (ascendingPhase && !transform->onGround && previousPos.y - transform->position.y < 2) {
		ascendingPhase = false;
		//std::cout << "hit accelerationPhase over" << std::endl;
		transform->velocity.y = Globalbilboulga::GRAVITY_STRENGTH;
	}
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