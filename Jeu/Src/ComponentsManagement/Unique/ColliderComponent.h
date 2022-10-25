#pragma once

#include <string>
#include <SDL2/SDL.h>

#include "TransformComponent.h"

class ColliderComponent : public Component
{
public:
	SDL_Rect collider, colliderSrc, texSrc, texDest;
	std::string tag;
	bool moving;
	bool flipped;
	bool* horizontalFlip;
	bool hasTex = false;
	SDL_Texture *texture;
	bool drawCollider = false;

	TransformComponent* transform;

	ColliderComponent(std::string t, bool m);
	ColliderComponent(std::string t, bool m, SDL_Rect rect);

	void init() override;
	void draw() override;
	void update() override;

	void setCollider(SDL_Rect rect);

private:
	bool* drawAllColliders = NULL;
};
