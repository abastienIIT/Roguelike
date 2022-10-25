#include "ColliderComponent.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Common/TextureManager.h"

ColliderComponent::ColliderComponent(std::string t, bool m)
{
	tag = t;
	moving = m;
	flipped = false;
}

ColliderComponent::ColliderComponent(std::string t, bool m, SDL_Rect rect)
{
	tag = t;
	moving = m;
	colliderSrc = { rect.x,rect.y,rect.w,rect.h };
	collider = colliderSrc;
	flipped = false;
}

void ColliderComponent::init()
{
	if (!entity->hasComponent<TransformComponent>())
	{
		entity->addComponent<TransformComponent>();
	}

	transform = &entity->getComponent<TransformComponent>();
	horizontalFlip = &transform->horizontalFlip;

	drawAllColliders = Globalbilboulga::getInstance()->getDrawAllColliders();
}

void ColliderComponent::draw()
{
	if (!drawCollider && !*drawAllColliders) return;

	SDL_Rect border;
	SDL_Rect posi;

	border.x = collider.x - Globalbilboulga::getInstance()->getCamera()->x;
	border.y = collider.y - Globalbilboulga::getInstance()->getCamera()->y;
	border.w = collider.w;
	border.h = collider.h;

	posi.x = transform->position.x - 1 - Globalbilboulga::getInstance()->getCamera()->x;
	posi.y = transform->position.y - 1 - Globalbilboulga::getInstance()->getCamera()->y;
	posi.w = 3;
	posi.h = 3;

	TextureManager::DrawRectangle(&border);
	TextureManager::DrawRectangle(&posi, 255, 0, 0);
}

void ColliderComponent::update()
{
	if (moving)
	{
		collider.x = transform->position.x + (colliderSrc.x - *horizontalFlip * (colliderSrc.w + 2 * colliderSrc.x)) * transform->scale;
		collider.y = transform->position.y + colliderSrc.y * transform->scale;
		collider.w = colliderSrc.w * transform->scale;
		collider.h = colliderSrc.h * transform->scale;
	}
}

void ColliderComponent::setCollider(SDL_Rect rect)
{
	colliderSrc = { rect.x,rect.y,rect.w,rect.h };
	collider = colliderSrc;
}
