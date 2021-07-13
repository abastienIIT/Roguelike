#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>

class Asset
{
public:
	Asset();
	Asset(std::string texturePath, int multiplier = 1);

	void addTexture(std::string texturePath);
	void setTexture(std::string texturePath, int pos);
	void delTexture(int pos);
	void setSizeMultiplier(int multiplier) { sizeMultiplier = multiplier; }

	std::vector<SDL_Texture*>* getAsset();
	SDL_Texture* getTexture(int pos);
	int getSizeMultiplier() { return sizeMultiplier; }

private:
	std::vector<SDL_Texture*> asset;

	int sizeMultiplier;
};
