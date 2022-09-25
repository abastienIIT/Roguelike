#include "TileComponent.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Common/TextureManager.h"
#include "../../Collisions/Collision.h"
#include <math.h>

TileComponent::TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine, bool isAnimated)
{
	position.x = xpos;
	position.y = ypos;

	srcRect.x = (id % texPerLine) * tileSize;
	srcRect.y = static_cast<int>(floor(id / texPerLine)) * tileSize;
	srcRect.w = srcRect.h = tileSize;

	destRect.x = xpos;
	destRect.y = ypos;
	destRect.w = destRect.h = tileSize * tileScale;

	this->isAnimated = isAnimated;

	if (!isAnimated)
	{
		tilesMap = Globalbilboulga::getInstance()->getAssetManager()->getTileSetAsset(idTex)->getTexture();
	}
	else
	{
		tilesMap = Globalbilboulga::getInstance()->getAssetManager()->getAnimatedTileSetAsset(idTex)->getTexture();

		nbFrames = Globalbilboulga::getInstance()->getAssetManager()->getAnimatedTileSetAsset(idTex)->getAnimationFrameNb(id);
		frameTime = Globalbilboulga::getInstance()->getAssetManager()->getAnimatedTileSetAsset(idTex)->getAnimationFrameTime(id);
	}

	camera = Globalbilboulga::getInstance()->getCamera();
}

void TileComponent::update()
{
	if (isAnimated)
	{
		srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / frameTime) % nbFrames);
	}
}

void TileComponent::draw()
{
	destRect.x = position.x - camera->x;
	destRect.y = position.y - camera->y;

	if (isOnScreen())
	{
		TextureManager::Draw(tilesMap, srcRect, destRect, SDL_FLIP_NONE);

		if (drawOutline)
		{
			SDL_Rect border;

			border.x = position.x - Globalbilboulga::getInstance()->getCamera()->x;
			border.y = position.y - Globalbilboulga::getInstance()->getCamera()->y;
			border.w = 64;
			border.h = 64;

			TextureManager::DrawRectangle(&border);

			drawOutline = false;
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
