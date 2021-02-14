#include "WeaponComponent.h"
#include "../Collisions/Collision.h"

void WeaponComponent::init()
{
	transform = &entity->getComponent<TransformComponent>();
	collider = &entity->getComponent<ColliderComponent>();
	colliderRect = &collider->collider;
	ownerPos = &owner->getComponent<TransformComponent>().position;
	ownerHand.x = ownerPos->x + 21 * 3;
	ownerHand.y = ownerPos->y - 4 * 3;
	transform->position.x = ownerPos->x + 21 * 3;

	transform->rotationCenter.x = 12;
	transform->rotationCenter.y = 63;
}

void WeaponComponent::update()
{
	if (owner->getComponent<TransformComponent>().velocity.x < 0)
	{
		ownerHand.x = ownerPos->x + 3 * 3;
		colliderRect->x = transform->position.x - colliderRect->w;
		ownerFacing = -1;
	}
	else if (owner->getComponent<TransformComponent>().velocity.x > 0)
	{
		ownerHand.x = ownerPos->x + 21 * 3;
		colliderRect->x = transform->position.x;
		ownerFacing = 1;
	}
	ownerHand.y = ownerPos->y - 4 * 3;
	transform->position.x = ownerHand.x;
	transform->position.y = ownerHand.y;
	colliderRect->y = ownerPos->y;

	if (attacking) attack();
}

void WeaponComponent::attack()
{
	unsigned int attackTime = 300;

	if (attacking)
	{
		transform->rotation = static_cast<int>(static_cast<float>(SDL_GetTicks() - attackStart) / attackTime * 90 * ownerFacing);

		for (auto& e : *targets)
		{
			if (Collision::AABB(*colliderRect, e->getComponent<ColliderComponent>().collider))
			{
				e->destroy();
				break;
			}
		}


		if (SDL_GetTicks() - attackStart > attackTime)
		{
			attacking = false;
			transform->rotation = 0;
		}
	}
	else
	{
		attacking = true;
		attackStart = SDL_GetTicks();
	}
}
