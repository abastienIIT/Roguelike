#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"

class TileComponent : public Component
{
public:
	std::vector<SDL_Texture*>* newtextures;
	SDL_Texture* texture;
	SDL_Rect srcRect, destRect;

	Vector2D position;

	TileComponent() = default;

	TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine)
	{
		newtextures = Globalbilboulga::getInstance()->getAssetManager()->getnewTexture(idTex);
		texture = Globalbilboulga::getInstance()->getAssetManager()->getTexture(idTex);

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
		destRect.x = position.x - Globalbilboulga::getInstance()->getCamera().x;
		destRect.y = position.y - Globalbilboulga::getInstance()->getCamera().y;
	}

	void draw() override
	{
		//TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
		for (auto& tex : *newtextures)
		{
			TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
		}
	}
};
