#pragma once

#include <map>
#include <string>
#include <SDL2/SDL.h>

#include "Components.h"
#include "../Common/TextureManager.h"
#include "Animation.h"
#include "../AssetManager.h"

class SpriteComponent: public Component
{
private:
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect src, dest;

	bool animated = false;
	int frames = 0;
	int speed = 100;

public:
	int animIndex = 0;

	std::map<std::string, Animation> animations;

	SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

	SpriteComponent() = default;
	SpriteComponent(std::string idTex)
	{
		setTex(idTex);
	}

	SpriteComponent(std::string idTex, bool isAnimated)
	{
		animated = isAnimated;

		if (animated)
		{
			animations = Game::assets->getAnim(idTex);
			std::map<std::string, Animation>::iterator it = animations.begin();
			play(it->first);
		}

		setTex(idTex);
	}

	~SpriteComponent()
	{
	}

	void setTex(std::string idTex)
	{
		texture = Game::assets->getTexture(idTex);
	}

	void init() override
	{
		transform = &entity->getComponent<TransformComponent>();

		src.h = transform->height;
		src.w = transform->width;
		src.x = src.y = 0;
	}

	void update() override
	{
		if (animated)
		{
			src.x = src.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
		}

		src.y = animIndex * transform->height;

		dest.x = transform->position.x - Game::camera.x;
		dest.y = transform->position.y - Game::camera.y;
		dest.w = transform->width * transform->scale;
		dest.h = transform->height * transform->scale;
	}

	void draw() override
	{
		if (transform->rotation == 0)
		{
			TextureManager::Draw(texture, src, dest, spriteFlip);
		}
		else
		{
			TextureManager::DrawRotate(texture, src, dest, spriteFlip, transform->rotation, transform->rotationCenter);
		}
	}

	void play(std::string animName)
	{
		frames = animations[animName].frames;
		animIndex = animations[animName].index;
		speed = animations[animName].speed;
	}
};
