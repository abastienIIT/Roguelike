#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"
#include "../../Common/Types/Vector2D.h"
#include "../../Common/Types/DoubleVector.h"

class TransformComponent : public Component
{
public:
	Vector2D position;
	DoubleVector velocity;

	double rotation = 0;
	SDL_Point rotationCenter;

	int speed = 5;

	int height = 32;
	int width = 32;
	int scale = 1;

	bool facingRight = true;
	bool applyGravity = false;
	double gravityCoef;
	bool falling = false;
	bool onGround = true;
	uint16_t gravity_pull_limit = 10;

	Vector2D previousPos;

	TransformComponent();
 	TransformComponent(int x, int y, int w, int h, int sc, bool mApplyGravity = false);

	void init() override;
	void update() override;
};
