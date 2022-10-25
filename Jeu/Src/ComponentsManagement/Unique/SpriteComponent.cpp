#include "SpriteComponent.h"

#include "../../Common/TextureManager.h"
#include "../../Common/Globalbilboulga.h"

SpriteComponent::SpriteComponent(std::string idAsset, int topLeftCornerX, int topLeftCornerY, int sizeMultiplier)
{
	this->topLeftCornerX = topLeftCornerX;
	this->topLeftCornerY = topLeftCornerY;
	this->sizeMultiplier = sizeMultiplier;
	setAsset(idAsset);
}

SpriteComponent::SpriteComponent(std::string idAsset, bool isAnimated, int topLeftCornerX, int topLeftCornerY, int sizeMultiplier)
{
	this->topLeftCornerX = topLeftCornerX;
	this->topLeftCornerY = topLeftCornerY;
	this->sizeMultiplier = sizeMultiplier;
	animated = isAnimated;

	if (isAnimated) setAnimatedAsset(idAsset);
	else setAsset(idAsset);
}

void SpriteComponent::setAsset(std::string idAsset, int slot)
{
	while (assets.size() < static_cast<size_t>(slot) + 1)
	{
		assets.emplace_back();
	}

	assets[slot] = Globalbilboulga::getInstance()->getAssetManager()->getAsset(idAsset);
}

void SpriteComponent::setAnimatedAsset(std::string idAsset, int slot)
{
	while (animatedAssets.size() < static_cast<size_t>(slot) + 1)
	{
		animatedAssets.emplace_back();
	}

	animatedAssets[slot] = Globalbilboulga::getInstance()->getAssetManager()->getAnimatedAsset(idAsset);
}

void SpriteComponent::init()
{
	transform = &entity->getComponent<TransformComponent>();

	if (assets.size() || animatedAssets.size()) setCurrentTexture(0);
}

void SpriteComponent::update()
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

void SpriteComponent::draw()
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

void SpriteComponent::play(std::string animName, int i)
{
	animation = animatedAssets[currentSlot]->getAsset()->at(i).second.at(animName);
	vframes[i] = animation.frames;
	vanimIndex[i] = animation.index;
	vspeed[i] = animation.speed;
	loops[i] = animation.loop;
}

void SpriteComponent::playCommon(std::string animName)
{
	for (int i = 0; i < nbTextures; i++)
	{
		play(animName, i);
	}
}

void SpriteComponent::playDefault()
{
	for (int i = 0; i < nbTextures; i++)
	{
		play(animatedAssets[currentSlot]->getDefaultAnim(i), i);
	}
}

void SpriteComponent::setCurrentTexture(int index)
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

			play(animatedAssets[index]->getDefaultAnim(i), i);
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

void SpriteComponent::setTopLeft(int x, int y)
{
	topLeftCornerX = x;
	topLeftCornerY = y;
}
