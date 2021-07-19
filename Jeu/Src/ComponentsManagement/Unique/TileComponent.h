#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"
#include "../../Common/Types/Asset.h"

class TileComponent : public Component
{
public:
	Asset* tilesMap;
	SDL_Rect srcRect, destRect;

	Vector2D position;

	TileComponent() = default;

	TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine)
	{
		tilesMap = Globalbilboulga::getInstance()->getAssetManager()->getAsset(idTex);

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
		for (SDL_Texture* tex : *tilesMap->getAsset())
		{
			TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
		}
	}
};
