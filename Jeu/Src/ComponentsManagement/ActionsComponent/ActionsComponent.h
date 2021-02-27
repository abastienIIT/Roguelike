#pragma once

#include "../Unique/ColliderComponent.h"
#include "../Unique/SpriteComponent.h"
#include "../../Common/Types/Vector2D.h"

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
	void jumpStop();
	void jumpStart();

	//Attacks
	void shootProjectile(Vector2D startPos, Vector2D velocity, SDL_Rect collider, int range, int speed, std::string idTex);
	void attackWeapon1Pressed();
	void attackWeapon1Realeased();
	void attackWeapon1SpecialPressed();
	void attackWeapon1SpecialRealeased();

	void attackWeapon2Pressed();
	void attackWeapon2Realeased();
	void attackWeapon2SpecialPressed();
	void attackWeapon2SpecialRealeased();

private:
	TransformComponent* transform;
	SpriteComponent* sprite;
	ColliderComponent* collider;
	Vector2D previousPos;

	bool accelerationPhase;
	bool onGround;
	bool falling;
	int jumpDuration;
	int startJumpY;
};
