#pragma once

#include <string>
#include <map>
#include <SDL2/SDL.h>

#include "../Common/Types/Vector2D.h"

class Area
{
public:
	Area(std::string area);
	~Area();

	void loadRoom(std::string path);
	void addTile(int idSrc, int x, int y);
	void addTile(int idSrc, int x, int y, SDL_Rect collider);

private:
	std::string area;
	std::string areaPath;
	Vector2D roomSize;
	int texPerLine;
	int roomScale;
	int tileSize;
	int scaledSize;
	bool hasColliders;

	std::map<int, SDL_Rect> colliders;
};
