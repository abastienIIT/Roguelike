#pragma once

#include "../CacWeapon.h"

class BKWeapon : public CacWeapon
{
public:
	BKWeapon() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);

	void update();

	void attackPressed(int attackID);
	void attackRealeased(int attackID);
	void attackInterrupt();

private:
	SDL_Rect AttackAboveHitbox = { 27,-3,17,24 };
	SDL_Rect AttackSideHitbox = { 29,9,21,4 };

	std::string currentAttack;
};

