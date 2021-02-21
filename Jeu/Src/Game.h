#pragma once

#define TESTMODE 0

#include <iostream>
#include <vector>
#include <FMOD/fmod.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Common/Globalbilboulga.h"
#include "Common/Types/Vector2D.h"
#include "ComponentsManagement/ECS.h"
#include "Area/Area.h"

class AssetManager;
class ColliderComponent;
class Entity;
class Globalbilboulga;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int channels);

	void update();
	void render();
	void clean();

	static bool isRunning;

	static Vector2D currentMapSize;

	static int gravityStrength;

	static AssetManager* assets;

	enum groupLabels : size_t
	{
		Maps,
		Players,
		Enemies,
		TerrainColliders,
		Projectiles,
		Weapons
	};

private:
	Globalbilboulga* globalbilboulga;

	std::map<groupLabels, std::vector<Entity*>*> entitiesGroups;

	Vector2D windowSize;

	Entity* player;

	Entity* label;

	Map* area1;
};
