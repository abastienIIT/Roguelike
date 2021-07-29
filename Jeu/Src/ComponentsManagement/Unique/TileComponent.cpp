#include "TileComponent.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Common/TextureManager.h"
#include "../../Collisions/Collision.h"

TileComponent::TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine)
{
	tilesMap = Globalbilboulga::getInstance()->getAssetManager()->getAsset(idTex);

	position.x = xpos;
	position.y = ypos;

	srcRect.x = (id % texPerLine) * tileSize;
	srcRect.y = static_cast<int>(floor(id / texPerLine)) * tileSize;
	srcRect.w = srcRect.h = tileSize;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = destRect.h = tileSize * tileScale;

	camera = Globalbilboulga::getInstance()->getCamera();
}

void TileComponent::update()
{
	destRect.x = position.x - camera->x;
	destRect.y = position.y - camera->y;
}

void TileComponent::draw()
{
	if (isOnScreen())
	{
		for (SDL_Texture* tex : *tilesMap->getAsset())
		{
			TextureManager::Draw(tex, srcRect, destRect, SDL_FLIP_NONE);
		}
	}
}

bool TileComponent::isOnScreen()
{
	if (Collision::AABB(SDL_Rect({ position.x, position.y, destRect.w, destRect.h }), *camera))
	{
		return true;
	}

    return false;
}
