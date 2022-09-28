#pragma once

#include <SDL2/SDL.h>
#include <string>

class TileSetAsset
{
public:
	TileSetAsset() { texture = nullptr; }
	TileSetAsset(std::string texturePath);

	SDL_Texture* getTexture() { return texture; }

protected:
	SDL_Texture* texture;
};

