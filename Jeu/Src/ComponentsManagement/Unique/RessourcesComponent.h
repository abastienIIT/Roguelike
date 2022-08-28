#pragma once

#include "TransformComponent.h"
#include "../ECS.h"

class RessourcesComponent : public Component
{
public:
	RessourcesComponent(int maxHealth = 100, int maxStamina = 0, int maxMana = 0, int maxAmmo = 0);

	~RessourcesComponent();

	void init();
	void update();
	void draw();

	bool takeDamage(int damage, int posX, int knockback = 0);
	void applyKnockback(int strength);

	TransformComponent* entityTransform = nullptr;

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

	bool intouchable = false;

	int knockbackLength = 0;
	int knockbackStrength = 0;
	int knockbackDirection = 1; //-1 to the left, 1 to the right
};
