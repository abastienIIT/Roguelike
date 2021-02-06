#pragma once

#include "Components.h"
#include "SDL.h"
#include "../AssetManager.h"

class TileComponent : public Component
{
public:
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	Vector2D position;

	TileComponent() = default;

	TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine)
	{
		texture = Game::assets->getTexture(idTex);

		position.x = xpos;
		position.y = ypos;

		srcRect.x = (id % texPerLine) * tileSize;
		srcRect.y =	static_cast<int>(floor(id/ texPerLine ))*tileSize;
		srcRect.w = srcRect.h = tileSize;

		destRect.x = xpos;
		destRect.y = ypos;
		destRect.w = destRect.h = tileSize * tileScale;
	}

	~TileComponent()
	{
	}

	void update() override
	{
		destRect.x = position.x - Game::camera.x;
		destRect.y = position.y - Game::camera.y;
	}

	void draw() override
	{
		TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
	}
};
