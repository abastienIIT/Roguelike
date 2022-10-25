#include "SpikeTrap.h"
#include "../../Unique/ColliderComponent.h"
#include "../../../Collisions/Collision.h"
#include "../../Unique/SpriteComponent.h"

void SpikeTrap::init(Entity* owner, std::vector<Entity*> targets)
{
	TrapBase::init(owner, targets);

	fullTrap.emplace_back(owner);

	damages = 3;
}

void SpikeTrap::update()
{
	if (trapState == states::Waiting && checkTargetOnTrap())
	{
		trapState = states::Triggering;
		triggerTime = SDL_GetTicks();

		playAnimForAllSections("Triggering");
	}
	else if (trapState == states::Triggering && SDL_GetTicks() - triggerTime > 1000)
	{
		trapState = states::Triggered;
		playAnimForAllSections("Triggered");
	}
	else if (trapState == states::Triggered)
	{
		if (!damageTargetsOnTrap(&owner->getComponent<ColliderComponent>().collider) && SDL_GetTicks() - triggerTime > 2000)
		{
			playAnimForAllSections("Reseting");
			trapState = states::Reseting;
			targetsHit.clear();
			triggerTime = SDL_GetTicks();
		}
	}
	else if (trapState == states::Reseting && SDL_GetTicks() - triggerTime > 300)
	{
		playAnimForAllSections("Wait");
		trapState = states::Waiting;
	}
}

void SpikeTrap::growTrap(Entity* newTrapSection, int widthToAdd)
{
	fullTrap.emplace_back(newTrapSection);

	ColliderComponent* trapCollider = &owner->getComponent<ColliderComponent>();

	trapCollider->setCollider(SDL_Rect{ trapCollider->collider.x, trapCollider->collider.y, trapCollider->collider.w + widthToAdd, trapCollider->collider.h });
}

bool SpikeTrap::checkTargetOnTrap()
{
	for (Entity* t : targets)
	{
		if (Collision::AABB(owner->getComponent<ColliderComponent>(), t->getComponent<ColliderComponent>()))
		{
			return true;
		}
	}

	return false;
}

void SpikeTrap::playAnimForAllSections(std::string animName)
{
	for (Entity* trapSection : fullTrap)
	{
		trapSection->getComponent<SpriteComponent>().play(animName);
		trapSection->getComponent<SpriteComponent>().animStart = SDL_GetTicks();
	}
}
