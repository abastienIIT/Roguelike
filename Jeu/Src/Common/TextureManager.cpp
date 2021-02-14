#include "TextureManager.h"
#include "Globalbilboulga.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
	SDL_Surface* surface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(globalbilboulga->getRenderer(), surface);
	SDL_FreeSurface(surface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
	SDL_RenderCopyEx(globalbilboulga->getRenderer(), tex, &src, &dest, 0, NULL, flip);
}

void TextureManager::DrawRotate(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle, SDL_Point center)
{
    Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
	SDL_RenderCopyEx(globalbilboulga->getRenderer(), tex, &src, &dest, angle, &center, flip);
}
