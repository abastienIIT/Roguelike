#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "../../Common/TextureManager.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Common/Types/Asset.h"
#include "../../Common/Types/AnimatedAsset.h"

class SpriteComponent: public Component
{
private:
	TransformComponent* transform;
	std::vector<Asset*> assets;
	std::vector<AnimatedAsset*> animatedAssets;

	int currentSlot = 0;
	std::vector<SDL_Rect> vsrc;
	SDL_Rect dest;
	
	bool animated = false;
	
	std::vector<std::string> defaultAnims;
	std::vector<int> vframes;
	std::vector<int> vspeed;
	std::vector<int> vanimIndex;

	std::vector<bool> loops;

	int sizeMultiplier = 1;

	std::vector<std::vector<std::vector<SDL_Texture*>*>> newtextures;
	std::vector<std::vector<SDL_Texture*>*> textures;
	std::string defaultAnim;
	int frames = 0;
	int speed = 100;
	int animIndex = 0;
	SDL_Rect src;
	SDL_Rect dest;
	

public:
	bool doubleSize = false;
	Uint32 animStart = 0;
	bool animLoop = true;

	std::vector<std::vector<std::map<std::string, Animation>>> newanimations;
	std::vector<std::map<std::string, Animation>> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string idAsset)
	{
		textures.emplace_back();
		setTex(idAsset);

		setAsset(idAsset);
	}

	SpriteComponent(std::string idAsset, bool isAnimated, std::string defaultAnimation = "")
	{
		animated = isAnimated;
		defaultAnim = defaultAnimation;

		if (animated)
		{
			animations.emplace_back();
			animations[0] = Globalbilboulga::getInstance()->getAssetManager()->getAnim(idAsset);
		}

		setTex(idAsset);

		if (isAnimated) setAnimatedAsset(idAsset);
		else setAsset(idAsset);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string idTex, int index = 0)
	{
		while (textures.size() < index + 1)
		{
			textures.emplace_back();

			if (animated) animations.emplace_back();
		}

		textures[index] = Globalbilboulga::getInstance()->getAssetManager()->getnewTexture(idTex);
		if (animated) animations[index] = Globalbilboulga::getInstance()->getAssetManager()->getAnim(idTex);
	}

	void newsetTex(std::string idTex, int index = 0, int slot = 0)
	{
		while (newtextures.size() < slot + 1)
		{
			newtextures.emplace_back();

			if (animated) newanimations.emplace_back();
		}

		while (newtextures[slot].size() < index + 1)
		{
			newtextures[slot].emplace_back();

			if (animated) newanimations[slot].emplace_back();
		}

		newtextures[slot][index] = Globalbilboulga::getInstance()->getAssetManager()->getnewTexture(idTex);
		if (animated) newanimations[slot][index] = Globalbilboulga::getInstance()->getAssetManager()->getAnim(idTex);
	}

	void setAsset(std::string idAsset, int slot = 0)
	{
		while (assets.size() < slot + 1)
		{
			assets.emplace_back();
		}

		assets[slot] = Globalbilboulga::getInstance()->getAssetManager()->getAsset(idAsset);
	}

	void setAnimatedAsset(std::string idAsset, int slot = 0)
	{
		while (animatedAssets.size() < slot + 1)
		{
			animatedAssets.emplace_back();
		}

		animatedAssets[slot] = Globalbilboulga::getInstance()->getAssetManager()->getAnimatedAsset(idAsset);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		src.h = transform->height;
		src.w = transform->width;
		src.x = src.y = 0;

		if (animated)
		{
			std::map<std::string, Animation>::iterator it = animations[0].begin();
			play(it->first);

			playDefault();
		}
	}

	void update() override
	{
		src.w = transform->width;
		src.h = transform->height;

		if (animated)
		{
			src.x = src.w * static_cast<int>(((SDL_GetTicks() - animStart) / speed) % frames);
			
			if (!animLoop)
			{
				if (static_cast<int>(((SDL_GetTicks() - animStart) / speed) / frames))
				{
					src.x = src.w * (frames - 1);
				}
			}
		}

		src.y = animIndex * transform->height;

		dest.x = transform->position.x - Globalbilboulga::getInstance()->getCamera().x;
		dest.y = transform->position.y - Globalbilboulga::getInstance()->getCamera().y + 1;
		dest.w = transform->width * transform->scale;
		dest.h = transform->height * transform->scale;

		if (doubleSize)
		{
			src.x *= 2;
			src.y *= 2;
			src.w *= 2;
			src.h *= 2;

			dest.x -= dest.w / 2;
			dest.y -= dest.h;
			dest.w *= 2;
			dest.h *= 2;
		}
	}

	void draw() override
	{
		if (transform->rotation == 0)
		{
			for (int i = 0; i < newtextures.at(currentSlot).size(); i++)
			{
				//TextureManager::Draw(tex, src, dest, spriteFlip);
				for (auto& tex : *newtextures.at(currentSlot).at(i))
				{
					TextureManager::Draw(tex, src, dest, spriteFlip);
				}
			}
		}
		else
		{
			for (auto& tex : *textures.at(currentSlot))
			{
				TextureManager::DrawRotate(tex, src, dest, spriteFlip, transform->rotation, transform->rotationCenter);
			}
		}
	}

	void play(std::string animName, int i = 0)
	{
		frames = animations[currentSlot][animName].frames;
		animIndex = animations[currentSlot][animName].index;
		speed = animations[currentSlot][animName].speed;
		animLoop = animations[currentSlot][animName].loop;

		Animation animation = animatedAssets[currentSlot]->getAsset()->at(i).second->at(animName);
		vframes[i] = animation.frames;
		vanimIndex[i] = animation.index;
		vspeed[i] = animation.speed;
		loops[i] = animation.loop;
	}

	void playDefault()
	{
		setCurrentTexture(0);

		animLoop = true;
		animIndex = 0;
	}

	void setCurrentTexture(int index) 
	{
		currentSlot = index;
		doubleSize = false;

		if (animated)
		{
			sizeMultiplier = animatedAssets[index]->getSizeMultiplier();

			vframes.clear();
			vanimIndex.clear();
			vspeed.clear();

			for (int i = 0; i < animatedAssets[index]->getAsset()->size(); i++)
			{
				vframes.emplace_back(0);
				vanimIndex.emplace_back(0);
				vspeed.emplace_back(100);
				loops[i] = true;

				play(animatedAssets[index]->getDefaultAnim(i));
			}
		}
		else
		{
			sizeMultiplier = assets[index]->getSizeMultiplier();
		}
	}
};
