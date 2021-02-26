#pragma once

#include "../WeaponSystem.h"

class BasicSword : public WeaponBase
{
public:
	BasicSword() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();
	void attackPressed();
	void attackRealeased();
	void attackSpecialPressed();
	void attackSpecialRealeased() {}

private:
	int attackStart = 0;
	bool attacking = false;
	bool attackHold = false;
	int lastAttack = 0;
	bool attack2 = false;
	bool attackRealeaseDone = false;
	bool lastAttack2 = false;
};
