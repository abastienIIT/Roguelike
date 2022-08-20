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
		Weapons
	};

private:
	Globalbilboulga* globalbilboulga;

	AssetManager* assets;

	int gravityStrength;

	Vector2D currentRoomSize;

	bool isRunning;

	std::map<groupLabels, std::vector<Entity*>*> entitiesGroups;

	Vector2D windowSize;

	Entity* player;

	Entity* labelPosition;
	Entity* labelVelocity;
	Entity* labelOnGround;
	Entity* labelPlayerHP;
	Entity* labelFPS;

	Area* area1;
};
