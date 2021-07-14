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

void AssetManager::addTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

void AssetManager::addAnimatedTexture(std::string id, const char* path, const char* pathAnim)
{
	textures.emplace(id, TextureManager::LoadTexture(path));

	char c;
	std::string num = "";
	std::string IDAnim = "";
	int index;
	int frames;
	int speed;
	std::map<std::string, Animation> ensemble;
	std::fstream animationInfos;
	animationInfos.open(pathAnim);

	animationInfos.get(c);
	while (c != '\n')
	{
		animationInfos.get(c);
	}

	animationInfos.get(c);
	while (c != '.')
	{
		IDAnim = "";
		while (c != ',')
		{
			IDAnim += c;
			animationInfos.get(c);
		}
		animationInfos.get(c);

		while (c != ',')
		{
			num += c;
			animationInfos.get(c);
		}
		index = stoi(num);

		num = "";
		animationInfos.get(c);

		while (c != ',')
		{
			num += c;
			animationInfos.get(c);
		}
		frames = stoi(num);

		num = "";
		animationInfos.get(c);

		while (c != '\n')
		{
			num += c;
			animationInfos.get(c);
		}
		speed = stoi(num);

		num = "";
		animationInfos.get(c);

		ensemble.emplace(IDAnim, Animation(index, frames, speed));
	}

	animations.emplace(id, ensemble);
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
	return textures[id];
}

std::map<std::string, Animation> AssetManager::getAnim(std::string id)
{
	return animations[id];
}

void AssetManager::addFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(std::string id)
{
	return fonts[id];
}
