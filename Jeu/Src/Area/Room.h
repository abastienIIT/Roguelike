#pragma once

#include <string>
#include <vector>
#include "../Common/Types/Vector2D.h"
#include "../ComponentsManagement/ECS.h"
#include "../ComponentsManagement/Unique/ColliderComponent.h"

#define ROOM_BASE_SIZE 16

class Room
{
public:
	Room(std::string roomName, std::string roomFolder, Vector2D roomSize, Vector2D roomPosition);

	std::string getName() { return roomName; }
	std::string getFolder() { return roomFolder; }
	Vector2D getRoomSize() { return roomSize; }
	Vector2D getRoomPosition() { return roomPosition; }

	void setExit(int pos, bool state) { exits.at(pos) = state; }
	void setAllExits(std::vector<bool> newExits) { exits = newExits; }
	void setExitsFromInt(int exitsInt);
	std::vector<bool>* getExits() { return &exits; }

	void addTile(Entity* tile) { tiles.emplace_back(tile); }
	void addTileCollider(Entity* tileCollider) { tilesCollider.emplace_back(tileCollider); }
	void addEnemie(Entity* enemie) { enemies.emplace_back(enemie); }
	void addTrap(Entity* trap) { traps.emplace_back(trap); }

	void removeEnemy(Entity* enemie) { enemies.erase(std::remove(enemies.begin(), enemies.end(), enemie), enemies.end()); }

	std::vector<Entity*>* getTiles() { return &tiles; }
	std::vector<Entity*>* getTilesCollider() { return &tilesCollider; }
	std::vector<Entity*>* getEnemies() { return &enemies; }
	std::vector<Entity*>* getTraps() { return &traps; }

	std::vector<std::vector<ColliderComponent*>>* getMapColliders() { return &mapColliders; }

private:
	std::string roomName;
	std::string roomFolder;
	Vector2D roomSize;
	Vector2D roomPosition;

	std::vector<bool> exits;

	std::vector<Entity*> tiles;
	std::vector<Entity*> tilesCollider;
	std::vector<Entity*> enemies;
	std::vector<Entity*> traps;

	std::vector<std::vector<ColliderComponent*>> mapColliders;
};

