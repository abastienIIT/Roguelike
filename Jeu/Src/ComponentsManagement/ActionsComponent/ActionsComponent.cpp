#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"
#include "../../Common/Globalbilboulga.h"

const int jumpHeight = 200;

ActionsComponent::ActionsComponent()
{
	jumping = false;
	gravity = true;
}

void ActionsComponent::update()
{
    transform->velocity.x = 0;
    sprite->playDefault();
	if (gravity)
	{
		transform->velocity.y = Globalbilboulga::getInstance()->getGravityStrength();
	}

	if (jumping)
	{
		jumpProcess();
	}

	previousPos = transform->position;
}

void ActionsComponent::walk(const int direction)
{
	if (!attacking)
	{
		transform->velocity.x = direction;

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
			}
			else
			{
				sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
				collider->flip(1);
			}
		}
	}
}

void ActionsComponent::jumpProcess()
{
	if ((transform->position.y < jumpStartPos - jumpHeight || previousPos.y == transform->position.y) && gravity == false)
	{
		gravity = true;
	}
	else if (previousPos.y == transform->position.y && gravity == true)
	{
		if (falling == true) jumping = false;
		falling = true;
	}
}

void ActionsComponent::jumpStart()
{
	if (!jumping && !attacking)
	{
		jumpStartPos = transform->position.y;
		transform->velocity.y = -Globalbilboulga::getInstance()->getGravityStrength();
		gravity = false;
		jumping = true;
		falling = false;
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