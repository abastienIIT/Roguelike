#pragma once

#include "../../ComponentsManagement/Animation/Animation.h"
#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include <map>

class AnimatedAsset
{
public:
	AnimatedAsset();
	AnimatedAsset(std::string texturePath, std::string animationPath, int multiplier = 1);

	void addTexture(std::string texturePath, std::string animationPath);
	void setTexture(std::string texturePath, std::string animationPath, int pos);
	void delTexture(int pos);
	void setSizeMultiplier(int multiplier) { sizeMultiplier = multiplier; }

	std::vector<std::pair<SDL_Texture*, std::map<std::string, Animation>*>>* getAsset();
	std::pair<SDL_Texture*, std::map<std::string, Animation>*>* getTexture(int pos);
	std::vector<std::string>* getDefaultAnims();
	std::string getDefaultAnim(int pos);
	int getSizeMultiplier() { return sizeMultiplier; }

	void createPair(std::string texturePath, std::string animationPath, int pos);
	std::map<std::string, Animation>* loadAnimation(std::string animationPath);

private:
	std::vector<std::pair<SDL_Texture*, std::map<std::string, Animation>*>> asset;
	std::vector<std::string> defaultAnims;

	std::string lastDefaultAnim;

	int sizeMultiplier;
};