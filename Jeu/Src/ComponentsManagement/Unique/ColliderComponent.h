#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "TransformComponent.h"
#include "../../Common/TextureManager.h"
#include "../../Common/Globalbilboulga.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider, colliderSrc, texSrc, texDest;
	std::string tag;
	bool moving;
	bool flipped;
	bool hasTex = false;
	SDL_Texture *texture;
	bool drawCollider = false;

	TransformComponent* transform;

	ColliderComponent(std::string t, bool m)
	{
		tag = t;
		moving = m;
		flipped = false;
	}

	ColliderComponent(std::string t, bool m, SDL_Rect rect)
	{
		tag = t;
		moving = m;
		colliderSrc = { rect.x,rect.y,rect.w,rect.h };
		collider = colliderSrc;
		flipped = false;
	}

	void init() override
	{
		if (!entity->hasComponent<TransformComponent>())
		{
			entity->addComponent<TransformComponent>();
		}

		transform = &entity->getComponent<TransformComponent>();

		drawAllColliders = Globalbilboulga::getInstance()->getDrawAllColliders();
	}

	void draw() override
	{
		if (!drawCollider && !*drawAllColliders) return;

		SDL_Rect border;

		border.x = collider.x - Globalbilboulga::getInstance()->getCamera()->x;
		border.y = collider.y - Globalbilboulga::getInstance()->getCamera()->y;
		border.w = collider.w;
		border.h = collider.h;
		
		TextureManager::DrawRectangle(&border);
	}

	void update() override
	{
		if (moving)
		{
			collider.x = transform->position.x + colliderSrc.x * transform->scale;
			collider.y = transform->position.y + colliderSrc.y * transform->scale;
			collider.w = colliderSrc.w * transform->scale;
			collider.h = colliderSrc.h * transform->scale;
		}
	}

	void flip(bool hFlip)
	{
		if (hFlip != flipped)
		{
			colliderSrc.x = 32 - colliderSrc.x - colliderSrc.w;
			flipped = hFlip;
		}
	}

	void setCollider(SDL_Rect rect)
	{
		colliderSrc = { rect.x,rect.y,rect.w,rect.h };
		collider = colliderSrc;
	}

private:
	bool* drawAllColliders = NULL;
};
