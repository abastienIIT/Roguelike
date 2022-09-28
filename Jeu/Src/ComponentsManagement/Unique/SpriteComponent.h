#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "../../Common/TextureManager.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Common/Types/Assets/Asset.h"
#include "../../Common/Types/Assets/AnimatedAsset.h"

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
	SpriteComponent(std::string idAsset, int topLeftCornerX = 0, int topLeftCornerY = 0, int sizeMultiplier = 1)
	{
		this->topLeftCornerX = topLeftCornerX;
		this->topLeftCornerY = topLeftCornerY;
		this->sizeMultiplier = sizeMultiplier;
		setAsset(idAsset);
	}

	SpriteComponent(std::string idAsset, bool isAnimated, int topLeftCornerX = 0, int topLeftCornerY = 0, int sizeMultiplier = 1)
	{
		this->topLeftCornerX = topLeftCornerX;
		this->topLeftCornerY = topLeftCornerY;
		this->sizeMultiplier = sizeMultiplier;
		animated = isAnimated;

		if (isAnimated) setAnimatedAsset(idAsset);
		else setAsset(idAsset);
	}

	~SpriteComponent()
	{
	}

	void animate(bool anim = true)
	{
		animated = anim;
	}

	void setAsset(std::string idAsset, int slot = 0)
	{
		while (assets.size() < static_cast<size_t>(slot) + 1)
		{
			assets.emplace_back();
		}

		assets[slot] = Globalbilboulga::getInstance()->getAssetManager()->getAsset(idAsset);
	}

	void setAnimatedAsset(std::string idAsset, int slot = 0)
	{
		while (animatedAssets.size() < static_cast<size_t>(slot) + 1)
		{
			animatedAssets.emplace_back();
		}

		animatedAssets[slot] = Globalbilboulga::getInstance()->getAssetManager()->getAnimatedAsset(idAsset);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		if (assets.size() || animatedAssets.size()) setCurrentTexture(0);
	}

	void update() override
	{
		if (animated)
		{
			for (int i = 0; i < nbTextures; i++)
			{
				if (!loops[i] && (static_cast<int>(((SDL_GetTicks() - animStart) / vspeed[i]) / vframes[i])))
				{
					vsrc[i].x = vsrc[i].w * (vframes[i] - 1);
				}
				else
				{
					vsrc[i].x = vsrc[i].w * static_cast<int>(((SDL_GetTicks() - animStart) / vspeed[i]) % vframes[i]);
				}

				vsrc[i].y = vsrc[i].h * vanimIndex[i];
			}
		}

		if (transform->horizontalFlip) spriteFlip = SDL_FLIP_HORIZONTAL;
		else spriteFlip = SDL_FLIP_NONE;
	}

	void draw() override
	{
		dest.x = transform->position.x - Globalbilboulga::getInstance()->getCamera()->x - (topLeftCornerX + transform->horizontalFlip * (transform->width - 2 * topLeftCornerX)) * transform->scale - (dest.w * (sizeMultiplier - 1) / (2 * sizeMultiplier));
		dest.y = transform->position.y - Globalbilboulga::getInstance()->getCamera()->y - topLeftCornerY * transform->scale - (dest.h * (sizeMultiplier - 1) / sizeMultiplier);

		SDL_Texture* toDraw;

		for (int i = 0; i < nbTextures; i++)
		{
			if (animated) toDraw = animatedAssets[currentSlot]->getTexture(i)->first;
			else toDraw = assets[currentSlot]->getTexture(i);

			if (transform->rotation == 0) TextureManager::Draw(toDraw, vsrc[i], dest, spriteFlip);
			else TextureManager::DrawRotate(toDraw, vsrc[i], dest, spriteFlip, transform->rotation, transform->rotationCenter);
		}
	}

	void play(std::string animName, int i = 0)
	{
		animation = animatedAssets[currentSlot]->getAsset()->at(i).second.at(animName);
		vframes[i] = animation.frames;
		vanimIndex[i] = animation.index;
		vspeed[i] = animation.speed;
		loops[i] = animation.loop;
	}

	void playCommon(std::string animName)
	{
		for (int i = 0; i < nbTextures; i++)
		{
			play(animName, i);
		}
	}

	void playDefault()
	{
		for (int i = 0; i < nbTextures; i++)
		{
			play(animatedAssets[currentSlot]->getDefaultAnim(i), i);
		}
	}

	void setCurrentTexture(int index) 
	{
		currentSlot = index;

		if (animated)
		{
			sizeMultiplier = animatedAssets[index]->getSizeMultiplier();
			nbTextures = static_cast<int>(animatedAssets[index]->getAsset()->size());
			animStart = SDL_GetTicks();

			vframes.clear();
			vanimIndex.clear();
			vspeed.clear();
			loops.clear();
			vsrc.clear();

			for (int i = 0; i < nbTextures; i++)
			{
				vframes.emplace_back(0);
				vanimIndex.emplace_back(0);
				vspeed.emplace_back(100);
				loops.emplace_back(true);

				play(animatedAssets[index]->getDefaultAnim(i),i);
			}
		}
		else
		{
			sizeMultiplier = assets[index]->getSizeMultiplier();
			nbTextures = static_cast<int>(assets[index]->getAsset()->size());
		}

		for (int i = 0; i < nbTextures; i++)
		{
			vsrc.emplace_back();
			vsrc[i].w = transform->width * sizeMultiplier;
			vsrc[i].h = transform->height * sizeMultiplier;
			vsrc[i].x = 0;
			vsrc[i].y = 0;
		}

		dest.w = transform->width * transform->scale * sizeMultiplier;
		dest.h = transform->height * transform->scale * sizeMultiplier;
	}

	void setTopLeft(int x, int y)
	{
		topLeftCornerX = x;
		topLeftCornerY = y;
	}
};
