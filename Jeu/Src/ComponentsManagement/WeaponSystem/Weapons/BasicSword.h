#pragma once

#include "../WeaponSystem.h"
#include "SDL2/SDL.h"
#include "../../Unique/TransformComponent.h"

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
	void attackInterrupt();

	void checkHit();

	void drawHitbox();

private:
	TransformComponent* transform;
	Vector2D* ownerPos;

	int damages = 60;

	int attackStart = 0;
	bool attacking = false;
	bool attackHold = false;
	int lastAttack = 0;
	bool attack2 = false;
	bool attackRealeaseDone = false;
	bool lastAttack2 = false;

	SDL_Rect hitboxAttack1 = { 12,16,19,3 };
	SDL_Rect hitboxAttack2 = { 13,12,18,3 };

	bool drawHB = false;
	bool* drawAllColliders;
	SDL_Rect hitboxToDraw;

	std::vector<Entity*> targetsHit;
};
