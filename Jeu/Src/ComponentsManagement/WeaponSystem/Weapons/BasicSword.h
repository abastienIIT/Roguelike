#pragma once

#include "../CacWeapon.h"
#include "SDL2/SDL.h"
#include "../../Unique/TransformComponent.h"

class BasicSword : public CacWeapon
{
public:
	BasicSword() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	void update();

	void attackPressed();
	void attackRealeased();
	void attackSpecialPressed();
	void attackSpecialRealeased() {}
	void attackInterrupt();


private:
	int lastAttack = 0;
	bool attack2 = false;
	bool lastAttack2 = false;

	SDL_Rect hitboxAttack1 = { 12,16,19,3 };
	SDL_Rect hitboxAttack2 = { 13,12,18,3 };
};
