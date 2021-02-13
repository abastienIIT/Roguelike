#include "Game.h"
#include "TextureManager.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include <typeinfo>
#include "AreaMap.h"

using namespace std;

Manager manager;

SDL_Renderer* Game::renderer = nullptr;
AssetManager* Game::assets = nullptr;

bool Game::isRunning = false;

Vector2D Game::currentMapSize;

SDL_Rect Game::camera = { 0,0,800,640 };

int Game::gravityStrength = 1;

int Game::FPS = 60;

Game::Game()
{}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		isRunning = false;
	}
	else
	{
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

		isRunning = true;
	}

	if (TTF_Init() == -1)
	{
		isRunning = false;
	}

	assets = new AssetManager(&manager);

	terrainColliders = &manager.getGroup(Game::groupTerrainColliders);
	enemies = &manager.getGroup(Game::groupEnemies);
	players = &manager.getGroup(Game::groupPlayers);
	tiles = &manager.getGroup(Game::groupMap);
	projectiles = &manager.getGroup(Game::groupProjectiles);
	weapons = &manager.getGroup(Game::groupWeapon);

	assets->addTexture("tilesArea1", "assets/Map/Area1/Tiles.png");
	assets->addAnimatedTexture("player", "assets/Player/Player.png", "assets/Player/PlayerInfos.txt");
	assets->addTexture("projectile", "assets/proj_test.png");
	assets->addAnimatedTexture("enemie", "assets/enemies/enemie.png", "assets/Enemies/EnemieInfos.txt");
	assets->addAnimatedTexture("giant", "assets/enemies/giant.png", "assets/Enemies/EnemieInfos.txt");
	assets->addTexture("sword", "assets/Epee.png");

	assets->addFont("LiberationSans-Regular", "assets/Fonts/LiberationSans-Regular.ttf", 16);

	SDL_GetWindowSize(window, &windowSize.x, &windowSize.y);

	assets->createPlayer();
	player = manager.getGroup(groupPlayers)[0];

	SDL_bool done = SDL_FALSE;

#if TESTMODE
	while (!done) {
		SDL_Event event;

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(renderer);

		std::vector<bool> exits;
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);
		exits.push_back(0);

		AreaMap::drawRoom(renderer, { 16,16,32,48 }, exits);
		AreaMap::drawRoom(renderer, { 100,100,16,16 }, exits);

		exits.clear();

		SDL_RenderPresent(renderer);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = SDL_TRUE;
			}
		}
	}
#endif // TESTMODE


	area1 = new Map("Area1");
	area1->LoadMap("0");
	Game::camera.w = Game::currentMapSize.x - windowSize.x;
	Game::camera.h = Game::currentMapSize.y - windowSize.y;

	label = assets->createLabel(Vector2D(10, 10), "LiberationSans-Regular", { 255,255,255,255 });
}

void Game::update()
{
	manager.refresh();
	manager.update();

	Collision::resolveCollisions(player, *terrainColliders);
	for (auto& e : *enemies)
	{
		Collision::resolveCollisions(e, *terrainColliders);
	}

	for (auto& p : *projectiles)
	{
		if (Collision::AABB(player->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			p->destroy();
		}
	}

	TransformComponent playerTransform = player->getComponent<TransformComponent>();

	if (playerTransform.position.x + playerTransform.width > Game::currentMapSize.x)
	{
		int mapNb = rand() % 4 + 1;
		//int mapNb = 4;
		switch (mapNb)
		{
		case 1:
			area1->LoadMap("1");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 1119;
			break;

		case 2:
			area1->LoadMap("2");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 576;
			break;

		case 3:
			area1->LoadMap("3");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 256;
			break;

		case 4:
			area1->LoadMap("4");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 192;
			break;
		}

		Game::camera.w = Game::currentMapSize.x - windowSize.x;
		Game::camera.h = Game::currentMapSize.y - windowSize.y;
	}

	camera.x = player->getComponent<TransformComponent>().position.x - Game::windowSize.x / 2;
	camera.y = player->getComponent<TransformComponent>().position.y - Game::windowSize.y / 2;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	std::stringstream ss;
	ss << "Player position : " << player->getComponent<TransformComponent>().position;
	label->getComponent<UILabel>().setLabelText(ss.str(), "LiberationSans-Regular");
}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& t : *tiles)
	{
		t->draw();
	}

	for (auto& p : *players)
	{
		p->draw();
	}

	for (auto& e : *enemies)
	{
		e->draw();
	}

	for (auto& p : *projectiles)
	{
		p->draw();
	}

	for (auto& w : *weapons)
	{
		w->draw();
	}

	label->draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
