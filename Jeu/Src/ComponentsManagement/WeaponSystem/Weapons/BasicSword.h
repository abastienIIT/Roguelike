#pragma once

#include "../CacWeapon.h"

class BasicSword : public CacWeapon
{
public:
	BasicSword() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();

	void attackPressed(int attackID);
	void attackRealeased(int attackID);
	void attackInterrupt();


private:
	int lastAttack = 0;
	bool attack2 = false;
	bool lastAttack2 = false;

	SDL_Rect hitboxAttack1 = { 14,2,19,3 };
	SDL_Rect hitboxAttack2 = { 15,-2,18,3 };
};
