#include "Asset.h"
#include "../TextureManager.h"

Asset::Asset(std::string texturePath, int multiplier)
{
	asset.emplace_back(TextureManager::LoadTexture(texturePath.c_str()));
	sizeMultiplier = multiplier;
}

void Asset::addTexture(std::string texturePath)
{
	asset.emplace_back(TextureManager::LoadTexture(texturePath.c_str()));
}

void Asset::setTexture(std::string texturePath, int pos)
{
	asset.emplace(asset.begin() + pos, TextureManager::LoadTexture(texturePath.c_str()));
}

void Asset::delTexture(int pos)
{
	asset.erase(asset.begin() + pos);
}

std::vector<SDL_Texture*>* Asset::getAsset()
{
	return &asset;
}

SDL_Texture* Asset::getTexture(int pos)
{
	return asset.at(pos);
}
