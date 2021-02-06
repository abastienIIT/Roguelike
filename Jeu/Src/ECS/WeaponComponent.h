#pragma once

#include "ECS.h"
#include "Components.h"
#include "../Vector2D.h"
#include <vector>

class WeaponComponent : public Component
{
public:
	WeaponComponent(Entity* owner, std::vector<Entity*>* targets) : owner(owner), targets(targets) {}
	~WeaponComponent() {}

	void init() override;
	void update() override;
	void draw() override {}

	void attack();
	bool isAttacking() { return attacking; }
	SDL_Rect* getCollider() { return colliderRect; }

private:
	Entity* owner;

	std::vector<Entity*>* targets;

	TransformComponent* transform;
	ColliderComponent* collider;
	SDL_Rect* colliderRect;
	Vector2D* ownerPos;

	Vector2D ownerHand;
	signed char ownerFacing = 1; //1 = facing right, -1 = facing left

	bool attacking = false;
	Uint32 attackStart;
};
