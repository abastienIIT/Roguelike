#include "Area.h"

#include <fstream>

#include "../Game.h"
#include "../ComponentsManagement/Components.h"
#include "TinyXML2/tinyxml2.h"
#include "../Collisions/Collision.h"

using namespace tinyxml2;

Area::Area(std::string area, Manager* manager)
{
	this->manager = manager;

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

	playerPosition = &manager->getGroup(Game::Players)[0]->getComponent<TransformComponent>().truePosition;
	playerHeigth = manager->getGroup(Game::Players)[0]->getComponent<TransformComponent>().height * manager->getGroup(Game::Players)[0]->getComponent<TransformComponent>().scale;
}

Area::~Area()
{

}

void Area::loadMap(std::string mapName)
{
	for (auto& t : manager->getGroup(Game::Maps)) t->destroy();
	for (auto& t : manager->getGroup(Game::TerrainColliders)) t->destroy();
	for (auto& e : manager->getGroup(Game::Enemies)) e->destroy();


	std::string mapFile = areaPath + "/" + mapName + "/" + mapName + ".tmx";

	firstgids.clear();

	XMLDocument doc;
	bool success = doc.LoadFile(mapFile.c_str());

	if (success)
	{
		std::cout << "Error loading map " + mapName << std::endl;
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
		else if (layerName == "Enemies") loadEnemies(&csvData);
		else if (layerName == "Utilities") loadUtilities(&csvData);
		else std::cout << "Wrong layer name in map " + mapName + " : " + layerName << std::endl;


		layer = layer->NextSiblingElement();

		if (!layer) break;
	}

	Globalbilboulga::getInstance()->setCurrentRoomSize(roomSize * scaledSize);
}

void Area::loadTiles(std::string* csvData, bool hasColliders)
{
	std::vector<std::vector<ColliderComponent*>>* mapCollider = &Globalbilboulga::getInstance()->mapColliders;
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
	std::vector<std::vector<ColliderComponent*>>* mapColliders = &Globalbilboulga::getInstance()->mapColliders;
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
				Globalbilboulga::getInstance()->getCharactereCreator()->createEnemies(idTile, Vector2D(x * scaledSize, y * scaledSize));
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

				default:
					std::cout << "Utilitie inconnue : " + idTile << std::endl;
				}
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
	auto& tile(manager->addEntity());
	tile.addComponent<TileComponent>(id, x, y, tileSize, roomScale, "tiles" + area, texPerLine);
	tile.addComponent<ColliderComponent>("terrain", false, SDL_Rect{0,0,0,0});
	tile.addGroup(Game::Maps);

	return &tile;
}

Entity* Area::addTile(int id, int x, int y, SDL_Rect collider)
{
	auto& tile(manager->addEntity());
	tile.addComponent<TileComponent>(id, x, y, tileSize, roomScale, "tiles" + area, texPerLine);
	tile.addComponent<ColliderComponent>("terrain", false, collider);

	tile.addGroup(Game::Maps);
	tile.addGroup(Game::TerrainColliders);

	return &tile;
}
