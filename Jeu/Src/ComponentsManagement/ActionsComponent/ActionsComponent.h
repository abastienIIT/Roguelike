#pragma once

#include "../Unique/ColliderComponent.h"
#include "../Unique/SpriteComponent.h"
#include "../../Common/Types/Vector2D.h"


class ActionsComponent : public Component
{
public:
	bool attacking = false;
	bool canMove;
	bool canRoll;
	bool canJump;

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
	void turnAround();
	void setPreviousPos(Vector2D pp) { previousPos = pp; }

	void jumpProcess();
	void jumpStop();
	void jumpStart();
	void resetJumpAtRoomChange(Vector2D playerSpawnPos);

	void roll();
	void rollProcess();
	bool canGetUp();

	//Attacks
	bool canShoot() { return true; }
	void attackPressed(int attackID, bool slot2 = false);
	void attackRealeased(int attackID, bool slot2 = false);
	void attackInterrupt();

private:
	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;
	ColliderComponent* collider = nullptr;
	Vector2D previousPos = { 0,0 };

	bool ascendingPhase = false;
	int startJumpY = 0;

	bool rolling = false;
	int rollStart = 0;
	double rollLoopTime = 0;
};
