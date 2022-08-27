#pragma once

#include "../ProjectileSystem.h"
#include "../../../Common/Types/Vector2D.h"

class Arrow : public ProjectileBase
{
public:
	Arrow() = default;
	Arrow(DoubleVector velocity, int strength, bool spriteFlipped, int damages) : initialVelocity(velocity), strength(strength), spriteFlipped(spriteFlipped), damages(damages) {}

	void init(Entity* projectile, std::vector<Entity*>* targets);
	void update();

	void targetHit(Entity* target);

private:
	DoubleVector initialVelocity;
	int strength;
	bool spriteFlipped;
	
	double distance = 0;
	int range = 50;
	int damages = 0;
	int knockback = 2;
};