#include "TextureManager.h"
#include "Globalbilboulga.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
	SDL_Surface* surface = IMG_Load(fileName);

	if (!surface)
	{
		std::cout << "Error loading image : " << fileName << std::endl;
		return nullptr;
	}

	SDL_Texture* tex = SDL_CreateTextureFromSurface(globalbilboulga->getRenderer(), surface);
	SDL_FreeSurface(surface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
	SDL_RenderCopyEx(Globalbilboulga::getInstance()->getRenderer(), tex, &src, &dest, 0, NULL, flip);
}

void TextureManager::DrawRotate(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle, SDL_Point center)
{
	SDL_RenderCopyEx(Globalbilboulga::getInstance()->getRenderer(), tex, &src, &dest, angle, &center, flip);
}

void TextureManager::DrawRectangle(SDL_Rect* rect, int r, int g, int b)
{
	SDL_Renderer* renderer = Globalbilboulga::getInstance()->getRenderer();
	SDL_SetRenderDrawColor(renderer, r, g, b, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
}
