#include "TransformComponent.h"

#include "../../Game.h"
#include "../ActionsComponent/ActionsComponent.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Collisions/Collision.h"

TransformComponent::TransformComponent()
{
	position.zero();
}

TransformComponent::TransformComponent(int x, int y, int w, int h, int sc, bool canMove, bool mApplyGravity, bool collidesWithGround)
{
	position.x = x;
	position.y = y;
	height = h;
	width = w;
	scale = sc;
	applyGravity = mApplyGravity;
	gravityCoef = 1;
	this->canMove = canMove;
	this->collidesWithGround = collidesWithGround;
}

void TransformComponent::init()
{
	gameSpeed = Globalbilboulga::getInstance()->getGameSpeed();
	truePosition.x = position.x;
	truePosition.y = position.y;
	velocity.zero();
	previousPos = position;
}

void TransformComponent::update()
{
	if (!canMove) return;

	truePosition.x += velocity.x * *gameSpeed;
	truePosition.y += velocity.y * *gameSpeed;

	position.x = (int)truePosition.x;
	position.y = (int)truePosition.y;

	
	if (collidesWithGround)
	{
		this->entity->getComponent<ColliderComponent>().update();
		Collision::resolveCollisions(this->entity);
	}
	

	if (!applyGravity)
		return;

	// Detect start falling (from edge or after ascending jump phase)
	if (!falling && position.y > previousPos.y) {
		falling = true;
		onGround = false;
	}
	// Detect landing
	else if (falling && previousPos.y == position.y) {
		onGround = true;
		falling = false;
	}

	// apply floor repulsion
	if (onGround)
	{
		//std::cout << "on ground" << std::endl;
		velocity.y = 1;
	}
	// apply gravity
	else if (velocity.y < gravity_pull_limit)
		velocity.y += Globalbilboulga::GRAVITY_STRENGTH * gravityCoef;

	previousPos = position;
}