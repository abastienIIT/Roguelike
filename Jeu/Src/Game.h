#pragma once

#define TESTMODE 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vector2D.h"
#include <iostream>
#include <vector>
#include "ECS/ECS.h"
#include "Map.h"

class AssetManager;
class ColliderComponent;
class Entity;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void update();
	void render();
	void clean();

	static SDL_Renderer* renderer;

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

	SDL_Window *window;
	Vector2D windowSize;

	Entity* player;

	Entity* label;

	Map* area1;
};
