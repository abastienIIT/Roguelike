#include <fstream>

#include "AssetManager.h"
#include "ComponentsManagement/Components.h"
#include "ComponentsManagement/IASystem/IAs.h"
#include "ComponentsManagement/WeaponSystem/Weapons.h"
#include "Common/TextureManager.h"
#include "Game.h"

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

Entity* AssetManager::createLabel(Vector2D pos, std::string policeName, SDL_Color color)
{
	auto& label(manager->addEntity());

	label.addComponent<UILabel>(pos, "Hello", policeName, color);

	return &label;
}



void AssetManager::addAsset(std::string id, Asset* asset)
{
	if (assets.find(id) == assets.end())
	{
		assets.emplace(std::make_pair(id, asset));
	}
}

void AssetManager::addAnimatedAsset(std::string id, AnimatedAsset* asset)
{
	if (animatedAssets.find(id) == animatedAssets.end())
	{
		animatedAssets.emplace(std::make_pair(id, asset));
	}
}

Asset* AssetManager::getAsset(std::string id)
{
	return assets.at(id);
}

AnimatedAsset* AssetManager::getAnimatedAsset(std::string id)
{
	return animatedAssets.at(id);
}

void AssetManager::addFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(std::string id)
{
	return fonts[id];
}
