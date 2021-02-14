#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "../Common/Globalbilboulga.h"
#include "../Common/Types/Vector2D.h"

class AreaMap
{
public:
	static void drawRoom(SDL_Rect roomDim, std::vector<bool> exits);

	static void drawLine(Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd);
	static void drawHalfLine(Vector2D roomPos, Vector2D lineStart, Vector2D lineEnd);
};
