#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>


class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* fileName);

	static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip);
	static void DrawRotate(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest, SDL_RendererFlip flip, double angle, SDL_Point center);

	static void DrawRectangle(SDL_Rect* rect, int r = 255, int g = 255, int b = 0);
};
