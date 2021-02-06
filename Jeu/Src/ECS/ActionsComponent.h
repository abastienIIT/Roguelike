#pragma once

#include "Components.h"
#include "../Vector2D.h"

class WeaponComponent;

class ActionsComponent : public Component
{
public:
	ActionsComponent();

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		sprite = &entity->getComponent<SpriteComponent>();
		collider = &entity->getComponent<ColliderComponent>();
	}

	void draw() override {}
	void update() override;

	//Movement
	void walk(const int direction);
	void jumpProcess();
	void jumpStart();

	//Attacks
	void shootProjectile(Vector2D startPos, Vector2D velocity, SDL_Rect collider, int range, int speed, std::string idTex);
	void swordAttack();

	//Set
	void setWeapon(WeaponComponent* weapon);

private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;
	WeaponComponent* weapon;

	Vector2D previousPos;

	bool gravity;

	bool jumping;
	int jumpStartPos;
};
