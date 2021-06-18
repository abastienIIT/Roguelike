#include "TransformComponent.h"

#include "../ActionsComponent/ActionsComponent.h"

TransformComponent::TransformComponent()
{
	position.zero();
}

TransformComponent::TransformComponent(int x, int y, int w, int h, int sc, bool mApplyGravity)
{
	position.x = x;
	position.y = y;
	height = h;
	width = w;
	scale = sc;
	applyGravity = mApplyGravity;
}

void TransformComponent::init()
{
	velocity.zero();
	previousPos = position;
}

void TransformComponent::update()
{
	//speed = static_cast<int>(3 * 60 / Game::FPS);
	position.x += velocity.x;
	position.y += velocity.y;

	if (!falling && position.y > previousPos.y ) {
		falling = true;
		onGround = false;
		std::cout << "Faling set true" << std::endl;
	}
	else if (falling && position.y == previousPos.y) {
		onGround = true;
		falling = false;
		std::cout << "Faling set false" << std::endl << std::endl;
	}

	// apply gravity
	if (applyGravity && velocity.y < MAX_GRAVITY_PULL)
		velocity.y += Globalbilboulga::GRAVITY_STRENGTH;

	previousPos = position;
}