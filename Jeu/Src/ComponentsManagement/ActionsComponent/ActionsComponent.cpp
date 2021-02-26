

#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"

const int jumpHeight = 200;

ActionsComponent::ActionsComponent()
{
	jumping = false;
	gravity = true;
}

void ActionsComponent::update()
{
    transform->velocity.x = 0;
    sprite->play("Idle");
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
	if (!jumping)
	{
		jumpStartPos = transform->position.y;
		transform->velocity.y = -Globalbilboulga::getInstance()->getGravityStrength();
		gravity = false;
		jumping = true;
		falling = false;
	}
}

void ActionsComponent::shootProjectile(Vector2D startPos, Vector2D velocity, SDL_Rect collider, int range, int speed, std::string idTex)
{
	Globalbilboulga::getInstance()->getAssetManager()->createProjectile(startPos, velocity, collider, range, speed, idTex);
}

void ActionsComponent::attackWeapon1Pressed()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>().attackPressed();
}

void ActionsComponent::attackWeapon1Realeased()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>().attackRealeased();
}

void ActionsComponent::attackWeapon1SpecialPressed()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>().attackSpecialPressed();
}

void ActionsComponent::attackWeapon1SpecialRealeased()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>().attackSpecialRealeased();
}


void ActionsComponent::attackWeapon2Pressed()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(true).attackPressed();
}

void ActionsComponent::attackWeapon2Realeased()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(true).attackRealeased();
}

void ActionsComponent::attackWeapon2SpecialPressed()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(true).attackSpecialPressed();
}

void ActionsComponent::attackWeapon2SpecialRealeased()
{
	entity->getComponent<WeaponComponent>().getWeapon<WeaponBase>(true).attackSpecialRealeased();
}
