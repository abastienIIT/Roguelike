#include "Game.h"
#include "TextureManager.h"
#include "Map.h"
#include "ECS/Components.h"
#include "Vector2D.h"
#include "Collision.h"
#include "AssetManager.h"
#include <sstream>
#include <typeinfo>

using namespace std;

SDL_Renderer* Game::renderer = nullptr;

Manager manager;
auto& player(manager.addEntity());
auto& label(manager.addEntity());

bool Game::isRunning = false;

SDL_Rect Game::camera = { 0,0,800,640 };
Vector2D Game::windowSize;

Vector2D Game::currentMapSize;

Map* area1;

int Game::gravityStrength = 1;

int Game::FPS = 60;

AssetManager* Game::assets = new AssetManager(&manager);

Entity* Game::playerPointer = &player;

auto& tiles(manager.getGroup(Game::groupMap));
auto& players(manager.getGroup(Game::groupPlayers));
auto& enemies(manager.getGroup(Game::groupEnemies));
auto& terrainColliders(manager.getGroup(Game::groupTerrainColliders));
auto& projectiles(manager.getGroup(Game::groupProjectiles));
auto& weapons(manager.getGroup(Game::groupWeapon));

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

	assets->addTexture("tilesArea1", "assets/Map/Area1/Tiles.png");
	assets->addAnimatedTexture("player", "assets/Player/Player.png", "assets/Player/PlayerInfos.txt");
	assets->addTexture("projectile", "assets/proj_test.png");
	assets->addAnimatedTexture("enemie", "assets/enemies/enemie.png", "assets/Enemies/EnemieInfos.txt");
	assets->addAnimatedTexture("giant", "assets/enemies/giant.png", "assets/Enemies/EnemieInfos.txt");
	assets->addTexture("sword", "assets/Epee.png");

	assets->addFont("arial", "assets/Fonts/arial.ttf", 16);

	SDL_GetWindowSize(window, &Game::windowSize.x, &Game::windowSize.y);

	area1 = new Map("Area1");
	area1->LoadMap("0");

	player.addComponent<TransformComponent>(0,863,32,32,3);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<ColliderComponent>("player", true, SDL_Rect({ 5,2,19,30 }));
	player.addComponent<ActionsComponent>();
	player.addComponent<InputController>();
	player.addGroup(groupPlayers);

	assets->createWeapon(&player, "", &enemies);

	SDL_Color white = { 255,255,255,255 };
	label.addComponent<UILabel>(Vector2D(10, 10), "Hello", "arial", white);
}

void Game::update()
{
	manager.refresh();
	manager.update();

	Collision::resolveCollisions(&player, terrainColliders);
	for (auto& e : enemies)
	{
		Collision::resolveCollisions(e, terrainColliders);
	}

	for (auto& p : projectiles)
	{
		if (Collision::AABB(player.getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			p->destroy();
		}
	}

	TransformComponent playerTransform = player.getComponent<TransformComponent>();

	if (playerTransform.position.x + playerTransform.width > Game::currentMapSize.x)
	{
		int mapNb = rand() % 4 + 1;
		//int mapNb = 4;
		switch (mapNb)
		{
		case 1:
			area1->LoadMap("1");
			player.getComponent<TransformComponent>().position.x = 0;
			player.getComponent<TransformComponent>().position.y = 1119;
			break;

		case 2:
			area1->LoadMap("2");
			player.getComponent<TransformComponent>().position.x = 0;
			player.getComponent<TransformComponent>().position.y = 576;
			break;

		case 3:
			area1->LoadMap("3");
			player.getComponent<TransformComponent>().position.x = 0;
			player.getComponent<TransformComponent>().position.y = 256;
			break;

		case 4:
			area1->LoadMap("4");
			player.getComponent<TransformComponent>().position.x = 0;
			player.getComponent<TransformComponent>().position.y = 192;
			break;
		}
	}

	camera.x = player.getComponent<TransformComponent>().position.x - Game::windowSize.x / 2;
	camera.y = player.getComponent<TransformComponent>().position.y - Game::windowSize.y / 2;

	if (camera.x < 0)
		camera.x = 0;
	if (camera.y < 0)
		camera.y = 0;
	if (camera.x > camera.w)
		camera.x = camera.w;
	if (camera.y > camera.h)
		camera.y = camera.h;

	std::stringstream ss;
	ss << "Player position : " << player.getComponent<TransformComponent>().position;
	label.getComponent<UILabel>().setLabelText(ss.str(), "arial");
}

void Game::render()
{
	SDL_RenderClear(renderer);

	for (auto& t : tiles)
	{
		t->draw();
	}

	for (auto& p : players)
	{
		p->draw();
	}

	for (auto& e : enemies)
	{
		e->draw();
	}

	for (auto& p : projectiles)
	{
		p->draw();
	}

	for (auto& w : weapons)
	{
		w->draw();
	}

	label.draw();

	SDL_RenderPresent(renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}
