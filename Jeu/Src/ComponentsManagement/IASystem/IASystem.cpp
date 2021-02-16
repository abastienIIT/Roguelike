#include "IAs.h"

Vector2D IABase::getDistanceToTarget()
{
	Vector2D dist = Vector2D({ transform->position.x - target->getComponent<TransformComponent>().position.x, transform->position.y - target->getComponent<TransformComponent>().position.y });
	return dist;
}