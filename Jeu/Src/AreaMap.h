#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include "Vector2D.h"

class AreaMap
{
public:
	static void drawRoom(SDL_Renderer* renderer, SDL_Rect roomDim, std::vector<bool> exits);

	static void drawLine(SDL_Renderer* renderer, Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd);
	static void drawHalfLine(SDL_Renderer* renderer, Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd);
};