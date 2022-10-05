#pragma once

#define TESTMODE 0

#include <iostream>
#include <vector>
#include <FMOD/fmod.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Common/Globalbilboulga.h"
#include "Common/Types/Vector2D.h"
#include "Area/Area.h"

class Game
{
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int channels);

	void update();
	void render();
	void clean();


	enum groupLabels : size_t
	{
		Maps,
		Players,
		Enemies,
		TerrainColliders,
		Projectiles,
		Weapons,
		Traps
	};

private:
	Globalbilboulga* globalbilboulga = nullptr;

	AssetManager* assets = nullptr;

	int gravityStrength = 0;

	Vector2D currentRoomSize = { 0,0 };

	bool isRunning = false;

	std::map<groupLabels, std::vector<Entity*>*> entitiesGroups;

	Vector2D windowSize = { 0,0 };

	Entity* player = nullptr;

	Entity* labelPosition = nullptr;
	Entity* labelVelocity = nullptr;
	Entity* labelOnGround = nullptr;
	Entity* labelPlayerHP = nullptr;
	Entity* labelFPS = nullptr;

	Area* area1 = nullptr;
};
