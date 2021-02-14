#pragma once

#include "../Globalbilboulga.h"
#include "ECS.h"
#include "../AssetManager.h"
#include "../TextureManager.h"
#include "../Game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

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
        Globalbilboulga = Globalbilboulga::getInstance();

		SDL_Surface* surface = TTF_RenderText_Blended(Game::assets->getFont(font), text.c_str(), textColor);
		labelTexture = SDL_CreateTextureFromSurface(Globalbilboulga->getRenderer(), surface);
		SDL_FreeSurface(surface);

		SDL_QueryTexture(labelTexture, nullptr, nullptr, &position.w, &position.h);
	}

	void draw() override
	{
        Globalbilboulga = Globalbilboulga::getInstance();
		SDL_RenderCopy(Globalbilboulga->getRenderer(), labelTexture, nullptr, &position);
	}

private:
    Globalbilboulga *Globalbilboulga;
	SDL_Rect position;
	std::string labelText;
	std::string labelFont;
	SDL_Color textColor;
	SDL_Texture* labelTexture;
};
