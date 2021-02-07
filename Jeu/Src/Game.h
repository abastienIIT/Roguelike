#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Vector2D.h"
#include <iostream>
#include <vector>

class AssetManager;
class ColliderComponent;
class Entity;

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvent();
	void update();
	void render();
	void clean();

	bool running() {return isRunning;};

	static SDL_Renderer* renderer;
	static SDL_Event event;

	static bool isRunning;

	static SDL_Rect camera;
	static Vector2D windowSize;

	static Vector2D currentMapSize;

	static int gravityStrength;

	static int FPS;

	static AssetManager* assets;

	static Entity* playerPointer;

	enum groupLabels : std::size_t
	{
		groupMap,
		groupPlayers,
		groupEnemies,
		groupTerrainColliders,
		groupProjectiles,
		groupWeapon
	};

private:
	SDL_Window *window;
};
