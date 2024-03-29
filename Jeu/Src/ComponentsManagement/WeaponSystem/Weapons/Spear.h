#pragma once

#include "../CacWeapon.h"

class Spear : public CacWeapon
{
public:
	Spear() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();

	void attackPressed(int attackID);
	void attackRealeased(int attackID);
	void attackInterrupt();
};

