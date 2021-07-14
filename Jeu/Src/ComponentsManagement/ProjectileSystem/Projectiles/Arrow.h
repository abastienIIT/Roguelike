#pragma once

#include "../ProjectileSystem.h"
#include "../../../Common/Types/Vector2D.h"

class Arrow : public ProjectileBase
{
public:
	Arrow() = default;
	Arrow(DoubleVector velocity, int strength, bool spriteFlipped) : initialVelocity(velocity), strength(strength), spriteFlipped(spriteFlipped) {}

	void init(Entity* projectile, std::vector<Entity*>* targets);
	void update();

private:
	DoubleVector initialVelocity;
	int strength;
	bool spriteFlipped;
	
	double distance = 0;
	int range = 50;
};