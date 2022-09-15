#pragma once

#include "../CacWeapon.h"

class Club : public CacWeapon
{
public:
	Club() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();

	void attackPressed(int attackID);
	void attackRealeased(int attackID);
	void attackInterrupt();

private:
	int knockbackBonk;
	int knockbackStomp;

	SDL_Rect AttackAboveHitbox = { 26,9,31,26 };
	SDL_Rect AttackSideHitbox = { 1,15,16,20 };

	std::string currentAttack;
};
