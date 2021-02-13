#include "Map.h"
#include "Game.h"
#include <fstream>
#include "ECS/ECS.h"
#include "ECS/Components.h"

extern Manager manager;

Map::Map(std::string area)
{
	this->area = area;
	areaPath = "assets/Map/" + this->area;

	std::fstream areaInfo;
	areaInfo.open(areaPath + "/AreaInfos.txt");

	char c =  '0';
	std::string num = "";

	//Sauter la première ligne
	while (c != '\n')
	{
		areaInfo.get(c);
	}

	//Récupérer premier nombre : nombre de texture par ligne
	areaInfo.get(c);
	while (c != ',' && c != '\n')
	{
		num += c;
		areaInfo.get(c);
	}
	this->texPerLine = stoi(num);
	num = "";

	//Récupérer deuxième nombre : scale des textures
	areaInfo.get(c);
	while (c != ',' && c != '\n')
	{
		num += c;
		areaInfo.get(c);
	}
	this->mapScale = stoi(num);
	num = "";

	//Récupérer troisième nombre : tailles des textures
	areaInfo.get(c);
	while (c != ',' && c != '\n')
	{
		num += c;
		areaInfo.get(c);
	}
	this->tileSize = stoi(num);
	num = "";

	this->scaledSize = mapScale * tileSize;

	//Construction de la liste des colliders
	areaInfo.get(c);
	while (c != '\n')
	{
		areaInfo.get(c);
	}

	int currentLine[5];
	SDL_Rect currentRect;

	areaInfo.get(c);
	while (c != '.')
	{
		for (int i = 0; i < 5; i++)
		{
			num = "";
			while (c != ',' && c != '\n')
			{
				num += c;
				areaInfo.get(c);
			}
			currentLine[i] = stoi(num);
			areaInfo.get(c);
		}
		currentRect = { currentLine[1] * mapScale, currentLine[2] * mapScale, currentLine[3] * mapScale, currentLine[4] * mapScale };
		colliders.insert(std::make_pair(currentLine[0], currentRect));
	}

	areaInfo.close();
}

Map::~Map()
{

}

void Map::LoadMap(std::string name)
{
	for (auto& t : manager.getGroup(Game::groupMap)) t->destroy();
	for (auto& t : manager.getGroup(Game::groupTerrainColliders)) t->destroy();
	for (auto& e : manager.getGroup(Game::groupEnemies)) e->destroy();

	int id;
	int enemieId;
	int enemieX;
	int enemieY;
	char c;
	std::string mapsSuffixes[2] = {"_BG","_FG"};
	std::string num = "";
	std::fstream mapInfos;
	std::fstream mapFile;
	hasColliders = false;

	std::string f = areaPath + "/" + name + "/mapInfos.txt";
	mapInfos.open(f);

	mapInfos.get(c);
	while (c != '\n')
	{
		mapInfos.get(c);
	}

	mapInfos.get(c);
	while (c != ',' && c != '\n')
	{
		num += c;
		mapInfos.get(c);
	}
	mapSize.x = stoi(num);
	num = "";

	mapInfos.get(c);
	while (c != ',' && c != '\n')
	{
		num += c;
		mapInfos.get(c);
	}
	mapSize.y = stoi(num);
	num = "";

	mapInfos.get(c);
	while (c != '\n')
	{
		mapInfos.get(c);
	}

	mapInfos.get(c);
	while (c != '.')
	{
		while (c != ',')
		{
			num += c;
			mapInfos.get(c);
		}
		enemieId = stoi(num);
		num = "";

		mapInfos.get(c);
		while (c != ',')
		{
			num += c;
			mapInfos.get(c);
		}
		enemieX = stoi(num) * mapScale;
		num = "";

		mapInfos.get(c);
		while (c != '\n')
		{
			num += c;
			mapInfos.get(c);
		}
		enemieY = stoi(num) * mapScale;
		num = "";

		Game::assets->createEnemies(enemieId, Vector2D(enemieX, enemieY));

		mapInfos.get(c);
	}

	mapInfos.close();


	for (std::string plan : mapsSuffixes)
	{
		mapFile.open(areaPath + "/" + name + "/" + name + plan + ".csv");

		if (plan == "_FG") hasColliders = true;
		else hasColliders = false;

		for (int y = 0; y < mapSize.y; y++)
		{
			for (int x = 0; x < mapSize.x; x++)
			{
				num = "";
				mapFile.get(c);

				while (c != ',' && c != '\n')
				{
					num += c;
					mapFile.get(c);
				}

				id = stoi(num);
				if (id != -1)
				{
					if (hasColliders)
					{
						addTile(id, x * scaledSize, y * scaledSize, { colliders[id].x + x * scaledSize, colliders[id].y + y * scaledSize, colliders[id].w, colliders[id].h });
					}
					else
					{
						addTile(id, x * scaledSize, y * scaledSize);
					}
				}
			}
		}

		mapFile.close();
	}

	Game::currentMapSize = mapSize * scaledSize;
}

void Map::addTile(int id, int x, int y)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(id, x, y, tileSize, mapScale, "tiles" + area, texPerLine);
	tile.addGroup(Game::groupMap);
}

void Map::addTile(int id, int x, int y, SDL_Rect collider)
{
	auto& tile(manager.addEntity());
	tile.addComponent<TileComponent>(id, x, y, tileSize, mapScale, "tiles" + area, texPerLine);
	tile.addComponent<ColliderComponent>("terrain", false, collider);

	tile.addGroup(Game::groupMap);
	tile.addGroup(Game::groupTerrainColliders);
}
