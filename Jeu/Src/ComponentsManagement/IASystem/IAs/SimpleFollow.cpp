#include "SimpleFollow.h"

void SimpleFollow::update()
{
	Vector2D entityPos = transform->position;
	Vector2D targetPos = focusedTarget->getComponent<TransformComponent>().position;
	if ((targetPos.x - entityPos.x > -200 && targetPos.x - entityPos.x < 200) && (targetPos.y - entityPos.y < 2000 && targetPos.y - entityPos.y > -2000))
	{
		if (targetPos.x > entityPos.x)
		{
			actions->walk(1);
		}
		else
		{
			actions->walk(-1);
		}
	}
	else transform->velocity.x = 0;
}
