#pragma once

#include <vector>
#include "Room.h"
#include "Area.h"

class Map
{
public:
	Map(Vector2D areaSize, Area* area, Manager* manager, Entity* player);

	void checkPlayerPosition();

	bool generateMap(std::string startName, Vector2D startPos, std::string endName, Vector2D endPos);
	void generateNextRoom(Vector2D pos);
	void regenerateClosestRoom(Vector2D pos);

	bool checkEveryExitIsOk(Room* roomToCheck);

	bool addRoom(Room* room);

	void deactivateCurrentRoom();
	void activateRoom(Room* room);

	bool canBeAdded(Vector2D roomPos, Vector2D roomSize);

	void displayMap();
	
private:
	Vector2D areaSize;
	Area* area;

	Entity* player;
	DoubleVector* playerPosition;

	Room* currentRoom;
	Vector2D currentRoomPos = { 0,0 };
	Vector2D currentRoomSize = { 0,0 };

	std::vector<std::vector<Room*>> roomMap;
	std::vector<Room*> roomList;
	std::vector<std::vector<std::vector<bool>>> exitMap;
	std::vector<Room*> specialsRoomsList;

	std::vector<std::pair<Vector2D, std::vector<std::pair<int, std::vector<std::string>>>>>* availableMaps;

	bool errorInMapGeneration = false;

	Manager* manager;
};

