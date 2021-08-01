#pragma once

#include <string>
#include <map>
#include <vector>
#include <SDL2/SDL.h>

#include "../ComponentsManagement/ECS.h"
#include "../Common/Types/Vector2D.h"
#include "../Common/Types/DoubleVector.h"

class Area
{
public:
	Area(std::string area, Manager* manager);
	~Area();

	void loadMap(std::string mapName);

	void loadTiles(std::string* csvData, bool hasColliders = 0);
	void loadEnemies(std::string* csvData);
	void loadUtilities(std::string* csvData);

	int getNextID(std::string* csvData);

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

	DoubleVector* playerPosition;
	int playerHeigth;

	std::vector<int> firstgids;
	int currentFirstgid = 0;

	Manager* manager;
};
