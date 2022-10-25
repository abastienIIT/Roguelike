#include "Area.h"

#include <fstream>
#include <filesystem>

#include "../Game.h"
#include "../ComponentsManagement/Components.h"
#include "TinyXML2/tinyxml2.h"
#include "../Collisions/Collision.h"

using namespace tinyxml2;

Area::Area(std::string area, Manager* manager)
{
	this->manager = manager;
	this->area = area;

	readAreaInfos();
	readAvailableMaps();
	loadAreaTextures();

	playerPosition = &manager->getGroup(Game::Players)[0]->getComponent<TransformComponent>().truePosition;
	playerHeigth = manager->getGroup(Game::Players)[0]->getComponent<TransformComponent>().height * manager->getGroup(Game::Players)[0]->getComponent<TransformComponent>().scale;
}

Area::~Area()
{

}

void Area::readAreaInfos()
{
	areaPath = "assets/Map/" + this->area;

	std::fstream areaInfo;
	areaInfo.open(areaPath + "/AreaInfos.txt");

	char c = '0';
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
	this->roomScale = stoi(num);
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

	this->scaledSize = roomScale * tileSize;
	Globalbilboulga::getInstance()->setCurrentTileSize(this->scaledSize);

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
		currentRect = { currentLine[1] * roomScale, currentLine[2] * roomScale, currentLine[3] * roomScale, currentLine[4] * roomScale };
		colliders.insert(std::make_pair(currentLine[0], currentRect));
	}

	areaInfo.close();
}

void Area::readAvailableMaps()
{
	const std::filesystem::path path{ "assets/Map/" + area + "/Maps" };

	std::string sizeFolder;
	Vector2D mapSize;
	std::vector<std::pair<int, std::vector<std::string>>> availableForSize;
	std::string shapeFolder;
	int shape;
	std::vector<std::string> roomNameList;
	
	for (const auto& entry : std::filesystem::directory_iterator(path))
	{
		sizeFolder = entry.path().u8string();
		
		if (sizeFolder == path.u8string() + "\\Specials")
			continue;

		int sizeX = stoi(sizeFolder.substr(sizeFolder.find("\\") + 1, sizeFolder.find("x") - sizeFolder.find("\\") - 1));
		int sizeY = stoi(sizeFolder.substr(sizeFolder.find("x") + 1, sizeFolder.length() - sizeFolder.find("x") - 1));
		mapSize = { sizeX / ROOM_BASE_SIZE, sizeY / ROOM_BASE_SIZE };


		availableForSize.clear();

		for (const auto& subEntry : std::filesystem::directory_iterator(entry.path()))
		{
			shapeFolder = subEntry.path().u8string();
			shape = stoi(shapeFolder.substr(shapeFolder.find("\\", shapeFolder.find("\\") + 1) + 1, shapeFolder.length() - shapeFolder.find("\\", shapeFolder.find("\\") + 1) - 1), nullptr, 2);

			roomNameList.clear();

			for (const auto& mapEntry : std::filesystem::directory_iterator(subEntry.path()))
			{
				std::string roomName = mapEntry.path().u8string();
				roomName = roomName.substr(roomName.find_last_of("\\") + 1, roomName.length() - roomName.find_last_of("\\") - 5);
				roomNameList.emplace_back(roomName);
			}

			availableForSize.emplace_back(std::make_pair(shape, roomNameList));
		}
		
		availableMaps.emplace_back(std::make_pair(mapSize, availableForSize));
	}

	//Print availableMaps
	/*
	for (auto v : availableMaps)
	{
		std::cout << "Map size : " << v.first << std::endl;

		for (auto v2 : v.second)
		{
			std::cout << v2.first << std::endl;

			for (auto v3 : v2.second)
			{
				std::cout << v3 << " ; ";
			}

			std::cout << std::endl;
		}
		
		std::cout << std::endl;
	}
	*/
}

void Area::loadAreaTextures()
{
	TileSetAsset* tiles = new TileSetAsset("assets/Map/" + area + "/Images/Tiles/Tiles.png");
	Globalbilboulga::getInstance()->getAssetManager()->addTileSetAsset("tilesArea1bis", tiles);

	AnimatedTileSetAsset* animatedTiles = new AnimatedTileSetAsset("assets/Map/" + area + "/Images/TilesAnimees/TilesAnimes.png", "assets/Map/" + area + "/Images/TilesAnimees/TilesAnimesInfos.txt");
	Globalbilboulga::getInstance()->getAssetManager()->addAnimatedTileSetAsset("animatedTilesArea1bis", animatedTiles);
}

void Area::loadRoom(Room* room)
{
	for (auto& t : manager->getGroup(Game::Maps)) t->destroy();
	for (auto& t : manager->getGroup(Game::TerrainColliders)) t->destroy();
	for (auto& e : manager->getGroup(Game::Enemies)) e->destroy();
	for (auto& t : manager->getGroup(Game::Traps)) t->destroy();

	currentRoom = room;
	std::string roomName = room->getName();

	std::string mapFile = areaPath + "/Maps/" + room->getFolder() + "/" + roomName + ".tmx";
	//std::cout << mapFile << std::endl;
	firstgids.clear();

	XMLDocument doc;
	bool success = doc.LoadFile(mapFile.c_str());

	if (success)
	{
		std::cout << "Error loading map " + roomName << std::endl;
		return;
	}

	XMLElement* map = doc.FirstChildElement("map");

	map->FindAttribute("width")->QueryIntValue(&roomSize.x);
	map->FindAttribute("height")->QueryIntValue(&roomSize.y);


	XMLElement* tileset = map->FirstChildElement("tileset");

	while (true)
	{
		const XMLAttribute* firstgid = tileset->FindAttribute("firstgid");

		if (!firstgid) break;

		firstgids.emplace_back(0);
		firstgid->QueryIntValue(&firstgids.back());
		tileset = tileset->NextSiblingElement();
	}


	XMLElement* layer = map->FirstChildElement("layer");
	XMLElement* data;
	std::string layerName;
	std::string csvData;

	while (true)
	{
		const XMLAttribute* name = layer->FindAttribute("name");

		if (!name) break;

		layerName = name->Value();
		
		data = layer->FirstChildElement("data");
		csvData = data->GetText();

		if (layerName == "BG") loadTilesNoCollider(&csvData);
		else if (layerName == "FG") loadTilesWithCollider(&csvData);
		else if (layerName == "Deco") loadAnimatedTiles(&csvData);
		else if (layerName == "Enemies") loadEnemies(&csvData);
		else if (layerName == "Utilities") loadUtilities(&csvData);
		else if (layerName == "Traps") loadTraps(&csvData);
		else std::cout << "Wrong layer name in map " + roomName + " : " + layerName << std::endl;


		layer = layer->NextSiblingElement();

		if (!layer) break;
	}
}

void Area::loadTiles(std::string* csvData, bool hasColliders)
{
	std::vector<std::vector<ColliderComponent*>>* mapCollider = Globalbilboulga::getInstance()->getCurrentMapColliders();
	mapCollider->clear();

	Entity* tileAdded = nullptr;
	int idTile;

	currentFirstgid = 0;

	for (int y = 0; y < roomSize.y; y++)
	{
		std::vector<ColliderComponent*> line(roomSize.x);
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;
				
				if (hasColliders)
				{
					tileAdded = addTile(idTile, x * scaledSize, y * scaledSize, { colliders[idTile].x + x * scaledSize, colliders[idTile].y + y * scaledSize, colliders[idTile].w, colliders[idTile].h });
					line[x] = &tileAdded->getComponent<ColliderComponent>();
				}
				else
				{
					tileAdded = addTile(idTile, x * scaledSize, y * scaledSize);
					line[x] = &tileAdded->getComponent<ColliderComponent>();
				}
			}
			else if (hasColliders)
			{
				line[x] = nullptr;
			}
		}

		mapCollider->emplace_back(line);
	}
}

void Area::loadTilesNoCollider(std::string* csvData)
{
	int idTile;

	currentFirstgid = 0;

	for (int y = 0; y < roomSize.y; y++)
	{
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;

				addTile(idTile, x * scaledSize, y * scaledSize);
			}
		}
	}
}

void Area::loadTilesWithCollider(std::string* csvData)
{
	std::vector<std::vector<ColliderComponent*>>* mapColliders = currentRoom->getMapColliders();
	mapColliders->clear();

	Entity* tileAdded = nullptr;
	int idTile;

	currentFirstgid = 0;

	for (int y = 0; y < roomSize.y; y++)
	{
		std::vector<ColliderComponent*> line(roomSize.x);
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;

				tileAdded = addTile(idTile, x * scaledSize, y * scaledSize, { colliders[idTile].x + x * scaledSize, colliders[idTile].y + y * scaledSize, colliders[idTile].w, colliders[idTile].h });
				line[x] = &tileAdded->getComponent<ColliderComponent>();
			}
			else
			{
				line[x] = nullptr;
			}
		}

		mapColliders->emplace_back(line);
	}
}

void Area::loadAnimatedTiles(std::string* csvData)
{
	int idTile;

	currentFirstgid = 0;

	for (int y = 0; y < roomSize.y; y++)
	{
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;

				addAnimatedTile(idTile, x * scaledSize, y * scaledSize);
			}
		}
	}
}

void Area::loadEnemies(std::string* csvData)
{
	int idTile;

	currentFirstgid = 0;

	for (int y = 0; y < roomSize.y; y++)
	{
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;
				Globalbilboulga::getInstance()->getCharactereCreator()->createEnemies(idTile, Vector2D(x * scaledSize, y * scaledSize), currentRoom);
			}
		}
	}
}

void Area::loadUtilities(std::string* csvData)
{
	int idTile;
	
	currentFirstgid = 0;

	for (int y = 0; y < roomSize.y; y++)
	{
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;
				switch (idTile)
				{
				case 0: //Tp du joueur aux coordonnées actuelles 
					*playerPosition = DoubleVector(x * scaledSize, (y + 1) * scaledSize - playerHeigth);
					break;

				case 1: //Exits
					addExitTile(x, y);
					break;

				default:
					std::cout << "Utilitie inconnue : " + idTile << std::endl;
				}
			}
		}
	}
}

void Area::loadTraps(std::string* csvData)
{
	int idTile;

	currentFirstgid = 0;

	Globalbilboulga::getInstance()->getTrapCreator()->initTrapMap(roomSize);

	for (int y = 0; y < roomSize.y; y++)
	{
		csvData->erase(0, 1);
		for (int x = 0; x < roomSize.x; x++)
		{
			idTile = getNextID(csvData);

			if (idTile)
			{
				idTile -= currentFirstgid;
				Globalbilboulga::getInstance()->getTrapCreator()->createTrap(idTile, Vector2D(x * scaledSize, y * scaledSize), currentRoom);
			}
		}
	}
}

int Area::getNextID(std::string* csvData)
{
	int idTile;
	std::string numRead = "";
	char c = csvData->at(0);

	csvData->erase(0, 1);

	while (c != ',' && c != '\n')
	{
		numRead += c;
		c = csvData->at(0);
		csvData->erase(0, 1);
	}

	idTile = stoi(numRead);

	if (idTile)
	{
		//Set du firstgid à utiliser
		if (!currentFirstgid)
		{
			int previousGid = 0;
			for (int gid : firstgids)
			{
				if (idTile < gid)
				{
					currentFirstgid = previousGid;
					break;
				}
				previousGid = gid;
			}

			if (!currentFirstgid) currentFirstgid = firstgids.back();
		}
	}
	
	return idTile;
}

Entity* Area::addTile(int id, int x, int y)
{
	auto& tile(manager->addEntity(false));
	tile.addComponent<TileComponent>(id, x, y, tileSize, roomScale, "tiles" + area, texPerLine);
	tile.setGroup(Game::Maps);

	currentRoom->addTile(&tile);

	return &tile;
}

Entity* Area::addTile(int id, int x, int y, SDL_Rect collider)
{
	auto& tile(manager->addEntity(false));
	tile.addComponent<TileComponent>(id, x, y, tileSize, roomScale, "tiles" + area, texPerLine);
	tile.addComponent<ColliderComponent>("terrain", false, collider);

	tile.setGroup(Game::Maps);
	tile.setGroup(Game::TerrainColliders);

	currentRoom->addTile(&tile);

	return &tile;
}

Entity* Area::addAnimatedTile(int id, int x, int y)
{
	auto& tile(manager->addEntity(false));
	tile.addComponent<TileComponent>(id, x, y, tileSize, roomScale, "animatedTiles" + area, 1, true);
	tile.setGroup(Game::Maps);

	currentRoom->addTile(&tile);

	return &tile;
}

Entity* Area::addExitTile(int x, int y)
{
	if (y == 0)
	{
		currentRoom->setExit(x / ROOM_BASE_SIZE, true);
	}
	else if (x == roomSize.x - 1)
	{
		currentRoom->setExit(roomSize.x / ROOM_BASE_SIZE + y / ROOM_BASE_SIZE, true);
	}
	else if (y == roomSize.y - 1)
	{
		currentRoom->setExit((roomSize.x + roomSize.y) / ROOM_BASE_SIZE + (roomSize.x / ROOM_BASE_SIZE - x / ROOM_BASE_SIZE) - 1, true);
	}
	else if (x == 0)
	{
		currentRoom->setExit((roomSize.x * 2 + roomSize.y) / ROOM_BASE_SIZE + (roomSize.y / ROOM_BASE_SIZE - y / ROOM_BASE_SIZE) - 1, true);
	}

	return nullptr;
}
