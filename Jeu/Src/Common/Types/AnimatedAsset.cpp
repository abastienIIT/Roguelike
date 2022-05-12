#include "AnimatedAsset.h"
#include "../TextureManager.h"
#include <fstream>
#include <iostream>

AnimatedAsset::AnimatedAsset(std::string texturePath, std::string animationPath, int multiplier)
{
	createPair(texturePath, animationPath, 0);
	sizeMultiplier = multiplier;
}

void AnimatedAsset::addTexture(std::string texturePath, std::string animationPath)
{
	int lastElem = (int)asset.size();
	createPair(texturePath, animationPath, lastElem);
}

void AnimatedAsset::setTexture(std::string texturePath, std::string animationPath, int pos)
{
	createPair(texturePath, animationPath, pos);
}

void AnimatedAsset::delTexture(int pos)
{
	asset.erase(asset.begin() + pos);
}

std::vector<std::pair<SDL_Texture*, std::map<std::string, Animation>>>* AnimatedAsset::getAsset()
{
	return &asset;
}

std::pair<SDL_Texture*, std::map<std::string, Animation>>* AnimatedAsset::getTexture(int pos)
{
	return &asset.at(pos);
}

std::vector<std::string>* AnimatedAsset::getDefaultAnims()
{
	return &defaultAnims;
}

std::string AnimatedAsset::getDefaultAnim(int pos)
{
	return defaultAnims.at(pos);
}

void AnimatedAsset::createPair(std::string texturePath, std::string animationPath, int pos)
{
	std::pair<SDL_Texture*, std::map<std::string, Animation>> pair;

	pair.first = TextureManager::LoadTexture(texturePath.c_str());
	pair.second = loadAnimation(animationPath);

	asset.emplace(asset.begin() + pos, pair); 

	defaultAnims.emplace(defaultAnims.begin() + pos, lastDefaultAnim);
}

std::map<std::string, Animation> AnimatedAsset::loadAnimation(std::string animationPath)
{
	char c;
	std::string num = "";
	std::string IDAnim = "";
	int index;
	int frames;
	int speed;
	bool loop;
	lastDefaultAnim = "";
	std::map<std::string, Animation> ensemble;
	std::fstream animationInfos;
	animationInfos.open(animationPath);

	if (!animationInfos.is_open())
	{
		std::cout << "Error loading animation data : " << animationPath << std::endl;
		return ensemble;
	}

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

		while (c != ',')
		{
			num += c;
			animationInfos.get(c);
		}
		speed = stoi(num);

		num = "";
		animationInfos.get(c);

		while (c != '\n')
		{
			num += c;
			animationInfos.get(c);
		}
		loop = stoi(num);

		num = "";
		animationInfos.get(c);
		
		ensemble.emplace(std::make_pair(IDAnim, Animation(index, frames, speed, loop)));

		if (lastDefaultAnim == "") lastDefaultAnim = IDAnim;
	}

	return ensemble;
}
