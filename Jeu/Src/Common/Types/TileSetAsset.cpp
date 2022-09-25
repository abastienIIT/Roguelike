#include "TileSetAsset.h"
#include "../TextureManager.h"

TileSetAsset::TileSetAsset(std::string texturePath)
{
	texture = TextureManager::LoadTexture(texturePath.c_str());
}
