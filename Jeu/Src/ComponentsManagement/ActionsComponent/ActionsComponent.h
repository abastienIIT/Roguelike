#pragma once

#include "../Unique/ColliderComponent.h"
#include "../Unique/SpriteComponent.h"
#include "../../Common/Types/Vector2D.h"


class ActionsComponent : public Component
{
public:
	bool attacking = false;

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
	bool canShoot() { return true; }
	void attackPressed(bool slot2 = false);
	void attackRealeased(bool slot2 = false);
	void attackSpecialPressed(bool slot2 = false);
	void attackSpecialRealeased(bool slot2 = false);

private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;

	Vector2D previousPos;

	bool gravity;

	bool jumping;
	bool falling;
	int jumpStartPos;
};
