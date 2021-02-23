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

Game::Game()
{
	globalbilboulga = Globalbilboulga::getInstance();
}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int channels)
{
    globalbilboulga->setIsRunning(true);
    int flags = 0;
    FMOD_SYSTEM *audioSystem;

    if(FMOD_System_Create(&audioSystem) != FMOD_OK)
    {
        globalbilboulga->setIsRunning(false);
    }

    if(FMOD_System_Init(audioSystem, channels, FMOD_INIT_NORMAL, NULL) != FMOD_OK)
    {
        globalbilboulga->setIsRunning(false);
    }

    globalbilboulga->setAudioSystem(audioSystem);

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
	   globalbilboulga->setIsRunning(false);
	}
	else
	{
		globalbilboulga->setWindow(SDL_CreateWindow(title, xpos, ypos, width, height, flags));
		globalbilboulga->setRenderer(SDL_CreateRenderer(globalbilboulga->getWindow(), -1, SDL_RENDERER_PRESENTVSYNC));
		globalbilboulga->setEventHandler(new EventHandler());
	}

	if (TTF_Init() == -1)
	{
		globalbilboulga->setIsRunning(false);
	}

	AssetManager* assets = nullptr;
	assets = new AssetManager(&manager);

	globalbilboulga->setAssetManager(assets);

	globalbilboulga->setGravityStrength(1);
	SDL_Rect camera = { 0,0,800,640 };
	globalbilboulga->setCamera(camera);

	for (char enumValue = Maps; enumValue <= Weapons; enumValue++) {
		groupLabels group = static_cast<groupLabels>(enumValue);
		entitiesGroups[group] = &manager.getGroup(enumValue);
	}

	globalbilboulga->getAssetManager()->addTexture("tilesArea1", "assets/Map/Area1/Tiles.png");
	globalbilboulga->getAssetManager()->addAnimatedTexture("player", "assets/Player/Player.png", "assets/Player/PlayerInfos.txt");
	globalbilboulga->getAssetManager()->addTexture("projectile", "assets/proj_test.png");
	globalbilboulga->getAssetManager()->addAnimatedTexture("enemie", "assets/enemies/enemie.png", "assets/Enemies/EnemieInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedTexture("giant", "assets/enemies/giant.png", "assets/Enemies/EnemieInfos.txt");
	globalbilboulga->getAssetManager()->addTexture("sword", "assets/Epee.png");

	globalbilboulga->getAssetManager()->addFont("LiberationSans-Regular", "assets/Fonts/LiberationSans-Regular.ttf", 16);

	SDL_GetWindowSize(globalbilboulga->getWindow(), &windowSize.x, &windowSize.y);

	globalbilboulga->getAssetManager()->createPlayer();
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


	area1 = new Area("Area1");
	area1->loadArea("0");
	globalbilboulga->setCameraW(globalbilboulga->getCurrentRoomSize().x - windowSize.x);
	globalbilboulga->setCameraH(globalbilboulga->getCurrentRoomSize().y - windowSize.y);

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

	if (playerTransform.position.x + playerTransform.width > globalbilboulga->getCurrentRoomSize().x)
	{
		int mapNb = rand() % 4 + 1;
		//int mapNb = 4;
		switch (mapNb)
		{
		case 1:
			area1->loadArea("1");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 1119;
			break;

		case 2:
			area1->loadArea("2");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 576;
			break;

		case 3:
			area1->loadArea("3");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 256;
			break;

		case 4:
			area1->loadArea("4");
			player->getComponent<TransformComponent>().position.x = 0;
			player->getComponent<TransformComponent>().position.y = 192;
			break;
		}

		globalbilboulga->setCameraW(globalbilboulga->getCurrentRoomSize().x - windowSize.x);
		globalbilboulga->setCameraH(globalbilboulga->getCurrentRoomSize().y - windowSize.y);
	}

	globalbilboulga->setCameraX(player->getComponent<TransformComponent>().position.x - windowSize.x / 2);
	globalbilboulga->setCameraY(player->getComponent<TransformComponent>().position.y - windowSize.y / 2);

	if (globalbilboulga->getCamera().x < 0)
		globalbilboulga->setCameraX(0);
	if (globalbilboulga->getCamera().y < 0)
		globalbilboulga->setCameraY(0);
	if (globalbilboulga->getCamera().x > globalbilboulga->getCamera().w)
		globalbilboulga->setCameraX(globalbilboulga->getCamera().w);
	if (globalbilboulga->getCamera().y > globalbilboulga->getCamera().h)
		globalbilboulga->setCameraY(globalbilboulga->getCamera().h);

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
    globalbilboulga->clean();
    Globalbilboulga::kill();
    TTF_Quit();
	SDL_Quit();
}
