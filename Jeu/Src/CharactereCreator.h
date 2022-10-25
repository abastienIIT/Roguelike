#pragma once

#include "ComponentsManagement/ECS.h"
#include "Common/Types/Vector2D.h"
#include "Area/Room.h"

class CharactereCreator
{
public:
	CharactereCreator(Manager* manager) : manager(manager) {};

	void createEnemies(int id, Vector2D pos, Room* room);

	void createPlayer();

private:
	Manager* manager;
};
