#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"
#include "../../Common/Types/Vector2D.h"
#include "../../Common/Globalbilboulga.h"

#define MAX_GRAVITY_PULL 10

class TransformComponent : public Component
{
public:
	Vector2D position;
	Vector2D velocity;

	double rotation = 0;
	SDL_Point rotationCenter;

	int speed = 5;

	int height = 32;
	int width = 32;
	int scale = 1;

	bool applyGravity = false;

	Vector2D previousPos;

	TransformComponent()
	{
		position.zero();
	}

	TransformComponent(int x, int y, int w, int h, int sc, bool mApplyGravity = false)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
		applyGravity = mApplyGravity;
	}

	void init() override
	{
		velocity.zero();
		previousPos = position;
	}

	void update() override
	{
		//speed = static_cast<int>(3 * 60 / Game::FPS);
		previousPos = position;
		position.x += velocity.x;
		position.y += velocity.y;

		// apply gravity
		if (applyGravity && velocity.y < MAX_GRAVITY_PULL)
			velocity.y += Globalbilboulga::GRAVITY_STRENGTH;
	}
};
