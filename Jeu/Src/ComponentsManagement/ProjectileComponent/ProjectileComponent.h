#pragma once

#include "../ECS.h"
#include "../Components.h"
#include "../../Common/Types/Vector2D.h"

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(int range, int speed, Vector2D velocity) : range(range), speed(speed), velocity(velocity)
	{}

	~ProjectileComponent() {}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();
		transform->velocity = velocity;
	}

	void update() override
	{
		distance += speed;

		if (distance > range ||
			transform->position.x < 0 ||
			transform->position.y < 0 ||
			transform->position.x > Globalbilboulga::getInstance()->getCurrentMapSize().x ||
			transform->position.y > Globalbilboulga::getInstance()->getCurrentMapSize().y)
		{
			entity->destroy();
		}
	}

	void draw() override
	{}


private:
	TransformComponent* transform;

	int range = 0;
	int speed = 0;
	int distance = 0;
	Vector2D velocity;
};
