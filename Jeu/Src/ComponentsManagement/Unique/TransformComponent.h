#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"
#include "../../Common/Types/Vector2D.h"

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

	Vector2D previousPos;

	bool facingRight = true;

	TransformComponent()
	{
		position.zero();
	}

	TransformComponent(int sc)
	{
		position.x = 0;
		position.y = 0;
		scale = sc;
	}

	TransformComponent(int x, int y)
	{
		position.x = x;
		position.y = y;
	}

	TransformComponent(int x, int y, int w, int h, int sc)
	{
		position.x = x;
		position.y = y;
		height = h;
		width = w;
		scale = sc;
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
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};
