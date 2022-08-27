#pragma once

#include "WeaponSystem.h"
#include "SDL2/SDL.h"
#include "../Unique/TransformComponent.h"

class CacWeapon : public WeaponBase
{
public:
	CacWeapon() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot);
	virtual void update() = 0;

	virtual void attackPressed(int attackID) = 0;
	virtual void attackRealeased(int attackID) = 0;
	virtual void attackInterrupt() = 0;

	void checkHit();

	void drawHitbox();

protected:
	TransformComponent* ownerTransform;
	Vector2D* ownerPos;

	std::string asset;

	int damages = 0;
	int knockback = 0;

	int attackStart = 0;
	bool attacking = false;
	bool attackHold = false;
	bool attackRealeaseDone = false;

	SDL_Rect hitboxDimensions;

	bool drawHB = false;
	bool* drawAllColliders;
	SDL_Rect hitboxToDraw;

	std::vector<Entity*> targetsHit;
};

