#include "StayStill.h"

void StayStill::update()
{
	Vector2D entityPos = transform->position;
	Vector2D targetPos = focusedTarget->getComponent<TransformComponent>().position;
	//std::cout << entityPos.x << " " << entityPos.y << "     " << owner->getComponent<ColliderComponent>().collider.x << " " << owner->getComponent<ColliderComponent>().collider.y << "     " << owner->getComponent<SpriteComponent>().dest.x << " " << owner->getComponent<SpriteComponent>().dest.y << std::endl;
}
