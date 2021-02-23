#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../../Common/Globalbilboulga.h"
#include "../ECS.h"
#include "../../AssetManager.h"
#include "../../Common/TextureManager.h"
#include "../../Game.h"

class UILabel : public Component
{
public:
	UILabel(Vector2D position, std::string text, std::string font, SDL_Color& color) :
		labelText(text), labelFont(font), textColor(color)
	{
		this->position.x = position.x;
		this->position.y = position.y;

		setLabelText(labelText, labelFont);
	}

	~UILabel()
	{}

	void setLabelText(std::string text, std::string font)
	{
        globalbilboulga = Globalbilboulga::getInstance();

		SDL_Surface* surface = TTF_RenderText_Blended(Globalbilboulga::getInstance()->getAssetManager()->getFont(font), text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(globalbilboulga->getRenderer(), surface);
		SDL_FreeSurface(surface);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override
	{
        globalbilboulga = Globalbilboulga::getInstance();
		SDL_RenderCopy(globalbilboulga->getRenderer(), labelTexture, nullptr, &position);
	}

private:
    Globalbilboulga *globalbilboulga;
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};
