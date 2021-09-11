#pragma once

#include "../ECS.h"

class RessourcesComponent : public Component
{
public:
	RessourcesComponent(int maxHealth = 100, int maxStamina = 0, int maxMana = 0, int maxAmmo = 0);

	~RessourcesComponent();

	void init();
	void update();
	void draw();

	void takeDamage(int damage) { health -= damage; }


	int health;
	int maxHealth;
	int regenHealth;
	int baseRegenHealth;

	int stamina;
	int maxStamina;
	int regenStamina;
	int baseRegenStamina;

	int mana;
	int maxMana;
	int regenMana;
	int baseRegenMana;

	int ammo;
	int maxAmmo;

	int lastRegen;
};
