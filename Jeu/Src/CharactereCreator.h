#pragma once

#include "ComponentsManagement/ECS.h"
#include "Common/Types/Vector2D.h"

class CharactereCreator
{
public:
	CharactereCreator(Manager* manager) : manager(manager) {};

	void createEnemies(int id, Vector2D pos);

	void createPlayer();

private:
	Manager* manager;
};
