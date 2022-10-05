#include "RessourcesComponent.h"
#include "../ActionsComponent/ActionsComponent.h"

#include "SDL2/SDL.h"

RessourcesComponent::RessourcesComponent(int maxHealth, int maxStamina, int maxMana, int maxAmmo)
{
	this->maxHealth = maxHealth;
	this->maxStamina = maxStamina;
	this->maxMana = maxMana;
	this->maxAmmo = maxAmmo;

	health = maxHealth;
	stamina = maxStamina;
	mana = maxMana;
	ammo = maxAmmo;

	baseRegenHealth = 0;
	baseRegenStamina = 0;
	baseRegenMana = 0;

	regenHealth = 0;
	regenStamina = 0;
	regenMana = 0;

	lastRegen = 0;
}

RessourcesComponent::~RessourcesComponent()
{
}

void RessourcesComponent::init()
{
	entityTransform = &entity->getComponent<TransformComponent>();
}

void RessourcesComponent::update()
{
	if (lastRegen < (int)(SDL_GetTicks() / 1000))
	{
		lastRegen = (int)(SDL_GetTicks() / 1000);

		health += regenHealth;
		stamina += regenStamina;
		mana += regenMana;
	}

	if (health > maxHealth) health = maxHealth;
	if (stamina > maxStamina) stamina = maxStamina;
	if (mana > maxMana) mana = maxMana;

	if (health <= 0) this->entity->destroy();

	if (knockbackLength)
	{
		entityTransform->velocity.x = static_cast<double>(4) * knockbackDirection * knockbackStrength;
		entityTransform->velocity.y = -2;

		if (knockbackLength == 1)
		{
			entity->getComponent<ActionsComponent>().canMove = true;
			entity->getComponent<ActionsComponent>().canRoll = true;
			entity->getComponent<ActionsComponent>().canJump = true;
		}

		knockbackLength--;
	}
}

void RessourcesComponent::draw()
{
}

bool RessourcesComponent::takeDamage(int damage, int posX, int knockback)
{
	if (!intouchable && SDL_GetTicks() - lastTimeDamageTaken > 200)
	{
		health -= damage;
		lastTimeDamageTaken = SDL_GetTicks();

		knockbackDirection = (entityTransform->position.x > posX) ? 1 : -1;
		if (knockback) applyKnockback(knockback);

		return true;
	}

	return false;
}

void RessourcesComponent::applyKnockback(int knockback)
{
	knockbackLength = 5 + knockback / 2;
	knockbackStrength = knockback / 2 + knockback % 2;
	entity->getComponent<ActionsComponent>().canMove = false;
	entity->getComponent<ActionsComponent>().canRoll = false;
	entity->getComponent<ActionsComponent>().canJump = false;
	entity->getComponent<ActionsComponent>().jumpStop();
}
