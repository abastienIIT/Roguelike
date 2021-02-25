#pragma once

#include "../WeaponSystem.h"

class BasicBow : public WeaponBase
{
public:
	BasicBow() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();
	void attackPressed();
	void attackRealeased();
	void attackSpecialPressed() {}
	void attackSpecialRealeased() {}

private:
	int attackStart = 0;
	bool attacking = false;
	bool attackHold = false;
	bool attackRealeaseDone = false;
};
