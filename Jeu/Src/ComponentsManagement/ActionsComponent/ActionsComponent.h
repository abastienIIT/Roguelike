#pragma once

#include "../Unique/ColliderComponent.h"
#include "../Unique/SpriteComponent.h"
#include "../../Common/Types/Vector2D.h"


class ActionsComponent : public Component
{
public:
	bool attacking = false;
	bool canMove;

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
	void jumpStop();
	void jumpStart();

	void roll();
	void rollProcess();
	bool canGetUp();

	//Attacks
	bool canShoot() { return true; }
	void attackPressed(bool slot2 = false);
	void attackRealeased(bool slot2 = false);
	void attackSpecialPressed(bool slot2 = false);
	void attackSpecialRealeased(bool slot2 = false);
	void attackInterrupt();

private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;
	Vector2D previousPos;

	bool ascendingPhase;
	int startJumpY;

	bool rolling;
	int rollStart;
	double rollLoopTime = 0;
};
