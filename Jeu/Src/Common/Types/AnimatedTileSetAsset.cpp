#include "AnimatedTileSetAsset.h"
#include "../TextureManager.h"
#include <fstream>
#include <iostream>

AnimatedTileSetAsset::AnimatedTileSetAsset(std::string texturePath, std::string animationPath)
{
	texture = TextureManager::LoadTexture(texturePath.c_str());

	loadAnimations(animationPath);
}

void AnimatedTileSetAsset::loadAnimations(std::string animationPath)
{
	char c;
	std::string num = "";
	std::string IDAnim = "";
	int index;
	int frames;
	int speed;
	bool loop;
	std::fstream animationInfos;
	animationInfos.open(animationPath);

	if (!animationInfos.is_open())
	{
		std::cout << "Error loading animation data : " << animationPath << std::endl;
		return;
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

		animations.emplace_back(Animation(index, frames, speed, loop));
	}
}