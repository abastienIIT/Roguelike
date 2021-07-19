#pragma once

#include "../ProjectileSystem.h"
#include "../../../Common/Types/Vector2D.h"

class Fireball : public ProjectileBase
{
public:
	Fireball() = default;
	Fireball(DoubleVector velocity) { this->velocity = velocity; }

	void init(Entity* projectile, std::vector<Entity*>* targets);
	void update();

private:
	int range = 1000;
	int distance = 0;
	DoubleVector velocity;
};