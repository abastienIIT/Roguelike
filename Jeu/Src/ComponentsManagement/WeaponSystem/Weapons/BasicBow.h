#pragma once

#include "../WeaponSystem.h"

class BasicBow : public WeaponBase
{
public:
	BasicBow() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();
	void attackPressed(int attackID);
	void attackRealeased(int attackID);
	void attackInterrupt();

private:
	int attackStart = 0;
	bool attacking = false;
	bool attackHold = false;
	bool attackRealeaseDone = false;

	int arrowDamage = 50;
};
