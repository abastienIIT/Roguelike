#include "IAs.h"

Vector2D IABase::getDistanceToTarget()
{
	Vector2D dist = Vector2D({ transform->position.x - focusedTarget->getComponent<TransformComponent>().position.x, transform->position.y - focusedTarget->getComponent<TransformComponent>().position.y });
	return dist;
}