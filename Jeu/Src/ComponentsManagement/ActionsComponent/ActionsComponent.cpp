

#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"

#define JUMP_DURATION 15 //frames
#define JUMP_ACCELARATION -2 // matchs gravity
#define JUMP_INITIAL_SPEED -13

ActionsComponent::ActionsComponent()
{
	jumpPressed = false;
	onGround = true;
	falling = false;
	jumpDuration = 0;
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
	transform->velocity.x = direction * 5;

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
	if (jumpPressed) {
		transform->velocity.y += JUMP_ACCELARATION;
		jumpDuration++;
	}

	if (jumpDuration > JUMP_DURATION)
		jumpPressed = false;

	if (!onGround && transform->position.y == previousPos.y) {
		if (!falling) {
			falling = true;
		}
		else {
			jumpPressed = false;
			onGround = true;
			falling = false;
			jumpDuration = 0;
		}
	}
}

void ActionsComponent::jumpStop()
{
	jumpPressed = false;
}

void ActionsComponent::jumpStart()
{
	if (onGround && !jumpPressed) {
		onGround = false;
		jumpPressed = true;
		transform->velocity.y = JUMP_INITIAL_SPEED;
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
	entity->getComponent<WeaponComponent>().getWeapon2<WeaponBase>().attackPressed();
}

void ActionsComponent::attackWeapon2Realeased()
{
	entity->getComponent<WeaponComponent>().getWeapon2<WeaponBase>().attackRealeased();
}

void ActionsComponent::attackWeapon2SpecialPressed()
{
	entity->getComponent<WeaponComponent>().getWeapon2<WeaponBase>().attackSpecialPressed();
}

void ActionsComponent::attackWeapon2SpecialRealeased()
{
	entity->getComponent<WeaponComponent>().getWeapon2<WeaponBase>().attackSpecialRealeased();
}
