#include "Input/InputController.h"
#include "ActionsComponent.h"

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
		transform->velocity.y = Game::gravityStrength;
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
		transform->velocity.y = -Game::gravityStrength;
		gravity = false;
		jumping = true;
		falling = false;
	}
}

void ActionsComponent::shootProjectile(Vector2D startPos, Vector2D velocity, SDL_Rect collider, int range, int speed, std::string idTex)
{
	Game::assets->createProjectile(startPos, velocity, collider, range, speed, idTex);
}

void ActionsComponent::swordAttack()
{
	weapon->attack();
}

void ActionsComponent::setWeapon(WeaponComponent* weapon)
{
	this->weapon = weapon;
}
