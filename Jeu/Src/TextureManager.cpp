#include "TextureManager.h"
#include "Globalbilboulga.h"

SDL_Texture* TextureManager::LoadTexture(const char* fileName)
{
    Globalbilboulga *Globalbilboulga = nullptr;
    Globalbilboulga = Globalbilboulga::getInstance();
	SDL_Surface* surface = IMG_Load(fileName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Globalbilboulga->getRenderer(), surface);
	SDL_FreeSurface(surface);

	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip)
{
    Globalbilboulga *Globalbilboulga = nullptr;
    Globalbilboulga = Globalbilboulga::getInstance();
	SDL_RenderCopyEx(Globalbilboulga->getRenderer(), tex, &src, &dest, NULL, NULL, flip);
}

void TextureManager::DrawRotate(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle, SDL_Point center)
{
    Globalbilboulga *Globalbilboulga = nullptr;
    Globalbilboulga = Globalbilboulga::getInstance();
	SDL_RenderCopyEx(Globalbilboulga->getRenderer(), tex, &src, &dest, angle, &center, flip);
}
