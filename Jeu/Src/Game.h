#pragma once

#define TESTMODE 0

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Common/Globalbilboulga.h"
#include "Common/Types/Vector2D.h"
#include <iostream>
#include <vector>
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

	void init(void);

	void update();
	void render();
	void clean();

	static bool isRunning;

	static Vector2D currentMapSize;

	static SDL_Rect camera;

	static int gravityStrength;

	static int FPS;

	static AssetManager* assets;

	enum groupLabels : std::size_t
	{
		Maps,
		Players,
		Enemies,
		TerrainColliders,
		Projectiles,
		Weapons
	};

private:
	std::map < groupLabels, std::vector<Entity*>*> entitiesGroups;
	Globalbilboulga* globalbilboulga;

	Vector2D windowSize;

	Entity* player;

	Entity* label;

	Area* area1;
};
