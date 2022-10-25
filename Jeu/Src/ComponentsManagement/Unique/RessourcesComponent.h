#pragma once

#include "TransformComponent.h"
#include "../ECS.h"
#include "../../Area/Room.h"

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

	void setEntityRoom(Room* room) { entityRoom = room; }

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
	int lastTimeDamageTaken = 0;

	int knockbackLength = 0;
	int knockbackStrength = 0;
	int knockbackDirection = 1; //-1 to the left, 1 to the right

	Room* entityRoom = nullptr;
};
