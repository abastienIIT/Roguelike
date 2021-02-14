#pragma once

#include <string>
#include <map>
#include <SDL2/SDL.h>

#include "../Common/Types/Vector2D.h"

class Map
{
public:
	Map(std::string area);
	~Map();

	void LoadMap(std::string path);
	void addTile(int idSrc, int x, int y);
	void addTile(int idSrc, int x, int y, SDL_Rect collider);

private:
	std::string area;
	std::string areaPath;
	Vector2D mapSize;
	int texPerLine;
	int mapScale;
	int tileSize;
	int scaledSize;
	bool hasColliders;

	std::map<int, SDL_Rect> colliders;
};
