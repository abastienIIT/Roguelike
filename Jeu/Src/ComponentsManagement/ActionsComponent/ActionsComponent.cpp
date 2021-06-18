

#include "ActionsComponent.h"
#include "../WeaponSystem/WeaponSystem.h"

#define JUMP_HEIGHT 230 //pixel
#define JUMP_ACCELARATION -Globalbilboulga::GRAVITY_STRENGTH
#define JUMP_INITIAL_SPEED -8

ActionsComponent::ActionsComponent()
{
	accelerationPhase = false;
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
	if (accelerationPhase) {
		transform->velocity.y += JUMP_ACCELARATION;
	}

	if (accelerationPhase && abs(startJumpY - transform->position.y) > JUMP_HEIGHT) {
		accelerationPhase = false;
		std::cout << "Heiht accelerationPhase over" << std::endl;
	}
	if (!transform->onGround && previousPos.y - transform->position.y < 8) {
		accelerationPhase = false;
		std::cout << "hit accelerationPhase over" << std::endl;
		transform->velocity.y = Globalbilboulga::GRAVITY_STRENGTH;
	}
}

void ActionsComponent::jumpStop()
{
	std::cout << "button accelerationPhase over" << std::endl;
	accelerationPhase = false;
}

void ActionsComponent::jumpStart()
{
	if (transform->onGround && !accelerationPhase) {
		accelerationPhase = true;
		transform->onGround = false;

		transform->velocity.y = JUMP_INITIAL_SPEED;
		startJumpY = transform->position.y;
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
