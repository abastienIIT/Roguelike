#pragma once

#include "ComponentsManagement/ECS.h"
#include "Common/Types/Vector2D.h"
#include "Area/Room.h"

class TrapCreator
{
public:
	TrapCreator(Manager* manager) : manager(manager) {};

	void initTrapMap(Vector2D roomSize);

	void createTrap(int id, Vector2D pos, Room* room);

private:
	Manager* manager;

	std::vector<std::vector<std::pair<int, Entity*>>> trapMap;
};

