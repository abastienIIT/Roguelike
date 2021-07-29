#pragma once

#include <SDL2/SDL.h>

#include "../ECS.h"
#include "../../Common/Types/Asset.h"
#include "../../Common/Types/Vector2D.h"

class TileComponent : public Component
{
private:
	Asset* tilesMap;
	SDL_Rect srcRect, destRect;

	Vector2D position;

	SDL_Rect* camera;

public:
	TileComponent() = default;

	TileComponent(int id, int xpos, int ypos, int tileSize, int tileScale, std::string idTex, int texPerLine);

	~TileComponent() {}

	void update() override;
	void draw() override;

	bool isOnScreen();
};
