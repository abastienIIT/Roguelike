#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>
#include <vector>

#include "../../Common/TextureManager.h"
#include "../../Common/Globalbilboulga.h"

class SpriteComponent: public Component
{
private:
	TransformComponent* transform;
	std::vector<SDL_Texture*> textures;
	int currentTexture = 0;
	SDL_Rect src, dest;
	std::string defaultAnim;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:
	bool doubleSize = false;
	Uint32 animStart = 0;
	int animIndex = 0;
	bool animLoop = true;

	std::vector<std::map<std::string, Animation>> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string idTex)
	{
		textures.emplace_back();
		setTex(idTex);
	}

	SpriteComponent(std::string idTex, bool isAnimated, std::string defaultAnimation = "")
	{
		animated = isAnimated;
		defaultAnim = defaultAnimation;
		textures.emplace_back();

		if (animated)
		{
			animations.emplace_back();
			animations[0] = Globalbilboulga::getInstance()->getAssetManager()->getAnim(idTex);
		}

		setTex(idTex);
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

		textures[index] = Globalbilboulga::getInstance()->getAssetManager()->getTexture(idTex);
		if (animated) animations[index] = Globalbilboulga::getInstance()->getAssetManager()->getAnim(idTex);
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
			TextureManager::Draw(textures[currentTexture], src, dest, spriteFlip);
		}
		else
		{
			TextureManager::DrawRotate(textures[currentTexture], src, dest, spriteFlip, transform->rotation, transform->rotationCenter);
		}
	}

	void play(std::string animName)
	{
		frames = animations[currentTexture][animName].frames;
		animIndex = animations[currentTexture][animName].index;
		speed = animations[currentTexture][animName].speed;
	}

	void playDefault()
	{
		currentTexture = 0;
		play(defaultAnim);
		doubleSize = false;
		animLoop = true;
	}

	void setCurrentTexture(int index) { currentTexture = index; }
};
