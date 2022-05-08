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

	void targetHit(Entity* target);

private:
	int range = 1000;
	double distance = 0;
	int damages = 0;

	DoubleVector velocity;
};