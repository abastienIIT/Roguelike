#include "Room.h"

Room::Room(std::string roomName, std::string roomFolder, Vector2D roomSize, Vector2D roomPosition)
{
	this->roomName = roomName;
	this->roomFolder = roomFolder;
	this->roomSize = roomSize;
	this->roomPosition = roomPosition;

	for (int i = 0; i < (roomSize.x + roomSize.y) * 2; i++)
		exits.emplace_back(false);
}

void Room::setExitsFromInt(int exitsInt)
{
	for (int i = exits.size() - 1; i >= 0; i--)
	{
		exits.at(i) = exitsInt & 1;
		exitsInt >>= 1;
	}
}
