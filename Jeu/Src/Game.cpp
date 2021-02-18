#include "Game.h"

#include <sstream>
#include <typeinfo>

#include "Common/TextureManager.h"
#include "ComponentsManagement/Components.h"
#include "Common/Types/Vector2D.h"
#include "Collisions/Collision.h"
#include "Area/AreaMap.h"

using namespace std;

Manager manager;

AssetManager* Game::assets = nullptr;

bool Game::isRunning = false;

Vector2D Game::currentMapSize;

SDL_Rect Game::camera = { 0,0,800,640 };

int Game::gravityStrength = 1;

Game::Game()
{
	globalbilboulga = Globalbilboulga::getInstance();
}

Game::~Game()
{}

void Game::init()
{
    isRunning = true;

	assets = new AssetManager(&manager);

	globalbilboulga->setAssetManager(assets);

	for (char enumValue = Maps; enumValue <= Weapons; enumValue++) {
		groupLabels group = static_cast<groupLabels>(enumValue);
		entitiesGroups[group] = &manager.getGroup(enumValue);
	}

	Globalbilboulga::getInstance()->getAssetManager()->addTexture("tilesArea1", "assets/Map/Area1/Tiles.png");
	//assets->addTexture("tilesArea1", "assets/Map/Area1/Tiles.png");
	assets->addAnimatedTexture("player", "assets/Player/Player.png", "assets/Player/PlayerInfos.txt");
	assets->addTexture("projectile", "assets/proj_test.png");
	assets->addAnimatedTexture("enemie", "assets/enemies/enemie.png", "assets/Enemies/EnemieInfos.txt");
	assets->addAnimatedTexture("giant", "assets/enemies/giant.png", "assets/Enemies/EnemieInfos.txt");
	assets->addTexture("sword", "assets/Epee.png");

	assets->addFont("LiberationSans-Regular", "assets/Fonts/LiberationSans-Regular.ttf", 16);

	SDL_GetWindowSize(globalbilboulga->getWindow(), &windowSize.x, &windowSize.y);

	assets->createPlayer();
	player = manager.getGroup(Players)[0];

#if TESTMODE
	SDL_bool done = SDL_FALSE;

	while (!done) {
		SDL_Event event;

		SDL_SetRenderDrawColor(globalbilboulga->getRenderer(), 255, 255, 255, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(globalbilboulga->getRenderer());

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

		AreaMap::drawRoom({ 16,16,32,48 }, exits);
		AreaMap::drawRoom({ 100,100,16,16 }, exits);

		exits.clear();

		SDL_RenderPresent(globalbilboulga->getRenderer());

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				done = SDL_TRUE;
			}
		}
	}
#endif // TESTMODE


	area1 = new Map("Area1");
	area1->LoadMap("0");
	camera.w = currentMapSize.x - windowSize.x;
	camera.h = currentMapSize.y - windowSize.y;

	label = assets->createLabel(Vector2D(10, 10), "LiberationSans-Regular", { 255,255,255,255 });
}

void Game::update()
{
	manager.refresh();
	manager.update();

	Collision::resolveCollisions(player, *entitiesGroups.at(TerrainColliders));
	for (auto& e : *entitiesGroups.at(Enemies))
	{
		Collision::resolveCollisions(e, *entitiesGroups.at(TerrainColliders));
	}

	for (auto& p : *entitiesGroups.at(Projectiles))
	{
		if (Collision::AABB(player->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
		{
			p->destroy();
		}
	}

	TransformComponent playerTransform = player->getComponent<TransformComponent>();

	if (playerTransform.position.x + playerTransform.width > currentMapSize.x)
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

		camera.w = currentMapSize.x - windowSize.x;
		camera.h = currentMapSize.y - windowSize.y;
	}

	camera.x = player->getComponent<TransformComponent>().position.x - windowSize.x / 2;
	camera.y = player->getComponent<TransformComponent>().position.y - windowSize.y / 2;

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
	SDL_RenderClear(globalbilboulga->getRenderer());

	for (pair<groupLabels, std::vector<Entity*>*> myPair : entitiesGroups) {
		if (myPair.first != TerrainColliders) {
			for(Entity* entitie : *myPair.second)
				entitie->draw();
		}
	}

	label->draw();

	SDL_RenderPresent(globalbilboulga->getRenderer());
}

void Game::clean()
{

}
