#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "../../Common/Types/Assets/Asset.h"
#include "../../Common/Types/Assets/AnimatedAsset.h"
#include "TransformComponent.h"

class SpriteComponent: public Component
{
private:
	TransformComponent* transform;
	std::vector<Asset*> assets;
	std::vector<AnimatedAsset*> animatedAssets;

	int topLeftCornerX;
	int topLeftCornerY;

	int currentSlot = 0;
	std::vector<SDL_Rect> vsrc;
	
	bool animated = false;

	Animation animation;
	
	std::vector<std::string> defaultAnims;
	std::vector<int> vframes;
	std::vector<int> vspeed;
	std::vector<int> vanimIndex;
	std::vector<bool> loops;

	int sizeMultiplier = 1;

	int nbTextures = 0;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

public:
	SDL_Rect dest;
	Uint32 animStart = 0;


	SpriteComponent() = default;
	SpriteComponent(std::string idAsset, int topLeftCornerX = 0, int topLeftCornerY = 0, int sizeMultiplier = 1);
	SpriteComponent(std::string idAsset, bool isAnimated, int topLeftCornerX = 0, int topLeftCornerY = 0, int sizeMultiplier = 1);

	~SpriteComponent() {}

	void animate(bool anim = true) { animated = anim; }

	void setAsset(std::string idAsset, int slot = 0);
	void setAnimatedAsset(std::string idAsset, int slot = 0);

	void init() override;
	void update() override;
	void draw() override;

	void play(std::string animName, int i = 0);
	void playCommon(std::string animName);
	void playDefault();

	void setCurrentTexture(int index);

	void setTopLeft(int x, int y);
};
