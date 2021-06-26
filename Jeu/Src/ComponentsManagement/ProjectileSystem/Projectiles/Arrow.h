#pragma once

#include "../ProjectileSystem.h"
#include "../../../Common/Types/Vector2D.h"

class Arrow : public ProjectileBase
{
public:
	Arrow() = default;
	Arrow(Vector2D velocity, int strength, bool spriteFlipped) : velocity(velocity), strength(strength), spriteFlipped(spriteFlipped) {}

	void init(Entity* projectile, std::vector<Entity*>* targets);
	void update();

private:
	Vector2D velocity;
	int strength;
	bool spriteFlipped;

	int distance = 0;
	int range = 50;
};