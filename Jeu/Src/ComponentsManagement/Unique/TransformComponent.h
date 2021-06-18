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
	bool falling = false;
	bool onGround;

	Vector2D previousPos;

	TransformComponent();

	TransformComponent(int x, int y, int w, int h, int sc, bool mApplyGravity = false);

	void init() override;
	void update() override;
};
