#pragma once

#include "../CacWeapon.h"

class Spear : public CacWeapon
{
public:
	Spear() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();

	void attackPressed();
	void attackRealeased();
	void attackSpecialPressed();
	void attackSpecialRealeased();
	void attackInterrupt();
};

