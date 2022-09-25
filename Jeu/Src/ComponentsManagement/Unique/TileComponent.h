#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"
#include "../../Common/Types/Vector2D.h"

class TileComponent : public Component
{
private:
	SDL_Texture* tilesMap;
	SDL_Rect srcRect, destRect;

	Vector2D position;

	SDL_Rect* camera;

	bool isAnimated;
	int nbFrames = 0;
	int frameTime = 0;

public:
	TileComponent() = default;

	TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine, bool isAnimated = false);

	~TileComponent() {}

	void update() override;
	void draw() override;

	bool isOnScreen();
	bool drawOutline = false;
};
