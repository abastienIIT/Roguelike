#include "RessourcesComponent.h"

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
}

void RessourcesComponent::draw()
{
}
