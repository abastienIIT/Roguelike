#pragma warning(disable : 26812)

#include "Game.h"

#include <sstream>
#include <typeinfo>


#include "Common/TextureManager.h"
#include "ComponentsManagement/Components.h"
#include "Common/Types/Vector2D.h"
#include "Collisions/Collision.h"
#include "Area/AreaMap.h"
#include "ProjectileCreator.h"
#include "TrapCreator.h"
#include "Common/Types/Assets/AnimatedAsset.h"
#include "Common/Types/Assets/Asset.h"
#include "Common/Types/Assets/AnimatedTileSetAsset.h"
#include "Common/Types/Assets/TileSetAsset.h"

using namespace std;

Manager manager;

Game::Game()
{
	globalbilboulga = Globalbilboulga::getInstance();
	globalbilboulga->setManager(&manager);
}

Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen, int channels)
{
    globalbilboulga->setIsRunning(true);
    int flags = 0;
    FMOD_SYSTEM *audioSystem;

    if(FMOD_System_Create(&audioSystem, FMOD_VERSION) != FMOD_OK)
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

	ProjectileCreator* projectileCreator = nullptr;
	projectileCreator = new ProjectileCreator(&manager);
	globalbilboulga->setProjectileCreator(projectileCreator);

	CharactereCreator* charactereCreator = nullptr;
	charactereCreator = new CharactereCreator(&manager);
	globalbilboulga->setCharactereCreator(charactereCreator);

	TrapCreator* trapCreator = nullptr;
	trapCreator = new TrapCreator(&manager);
	globalbilboulga->setTrapCreator(trapCreator);

	//globalbilboulga->setGravityStrength(1);
	SDL_Rect camera = { 0,0,800,640 };
	globalbilboulga->setCamera(camera);

	for (char enumValue = Maps; enumValue <= Traps; enumValue++) {
		groupLabels group = static_cast<groupLabels>(enumValue);
		entitiesGroups[group] = &manager.getGroup(enumValue);
	}

	AnimatedAsset* playerBase = new AnimatedAsset("assets/Player/PlayerBot.png", "assets/Player/PlayerInfos.txt");
	playerBase->addTexture("assets/Player/PlayerTop.png", "assets/Player/PlayerInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("playerBase", playerBase);

	AnimatedAsset* playerSword = new AnimatedAsset("assets/Player/Weapons/BasicSword/BasicSwordBot.png", "assets/Player/Weapons/BasicSword/InfosBot.txt", 2);
	playerSword->addTexture("assets/Player/Weapons/BasicSword/BasicSwordTop.png", "assets/Player/Weapons/BasicSword/InfosTop.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("BasicSword", playerSword);

	AnimatedAsset* playerBow = new AnimatedAsset("assets/Player/Weapons/BasicBow/BasicBowBot.png", "assets/Player/Weapons/BasicBow/InfosBot.txt");
	playerBow->addTexture("assets/Player/Weapons/BasicBow/BasicBowTop.png", "assets/Player/Weapons/BasicBow/InfosTop.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("BasicBow", playerBow);

	AnimatedAsset* spear = new AnimatedAsset("assets/Player/Weapons/Spear/SpearBot.png", "assets/Player/Weapons/Spear/InfosBot.txt", 2);
	spear->addTexture("assets/Player/Weapons/Spear/SpearTop.png", "assets/Player/Weapons/Spear/InfosTop.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("Spear", spear);

	AnimatedAsset* enemie = new AnimatedAsset("assets/enemies/enemie.png", "assets/Enemies/EnemieInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("enemie", enemie);

	AnimatedAsset* giant = new AnimatedAsset("assets/enemies/Giant/Giant.png", "assets/Enemies/Giant/GiantInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("giant", giant);

	AnimatedAsset* bigKnight = new AnimatedAsset("assets/Enemies/Knight/KnightFull.png", "assets/Enemies/Knight/KnightInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("BigKnight", bigKnight);

	Asset* fireball = new Asset("assets/proj_test.png");
	globalbilboulga->getAssetManager()->addAsset("Fireball", fireball);

	Asset* arrow = new Asset("assets/Projectiles/Arrow.png");
	globalbilboulga->getAssetManager()->addAsset("Arrow", arrow);

	TileSetAsset* tiles = new TileSetAsset("assets/Map/Area1/Tiles.png");
	globalbilboulga->getAssetManager()->addTileSetAsset("tilesArea1", tiles);

	AnimatedTileSetAsset* animatedTiles = new AnimatedTileSetAsset("assets/Map/Area1/TilesAnimes.png", "assets/Map/Area1/TilesAnimesInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedTileSetAsset("animatedTilesArea1", animatedTiles);

	AnimatedAsset* spikeTrapRight = new AnimatedAsset("assets/Traps/SpikeTrap/SpikeTrapRight.png", "assets/Traps/SpikeTrap/SpikeTrapInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("spikeTrapRight", spikeTrapRight);

	AnimatedAsset* spikeTrapCenter = new AnimatedAsset("assets/Traps/SpikeTrap/SpikeTrapCenter.png", "assets/Traps/SpikeTrap/SpikeTrapInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("spikeTrapCenter", spikeTrapCenter);

	AnimatedAsset* spikeTrapLeft = new AnimatedAsset("assets/Traps/SpikeTrap/SpikeTrapLeft.png", "assets/Traps/SpikeTrap/SpikeTrapInfos.txt");
	globalbilboulga->getAssetManager()->addAnimatedAsset("spikeTrapLeft", spikeTrapLeft);

	globalbilboulga->getAssetManager()->addFont("LiberationSans-Regular", "assets/Fonts/LiberationSans-Regular.ttf", 16);

	SDL_GetWindowSize(globalbilboulga->getWindow(), &windowSize.x, &windowSize.y);
	*globalbilboulga->getWindowSize() = windowSize;

	globalbilboulga->getCharactereCreator()->createPlayer();
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


	area1 = new Area("Area1",globalbilboulga->getManager());
	area1->loadMap("0");
	//area1->loadMap("Arena");
	globalbilboulga->setCameraW(windowSize.x);
	globalbilboulga->setCameraH(windowSize.y);

	labelPosition = assets->createLabel(Vector2D(10, 10), "LiberationSans-Regular", { 255,255,255,255 });
	labelVelocity = assets->createLabel(Vector2D(10, 40), "LiberationSans-Regular", { 255,255,255,255 });
	labelOnGround = assets->createLabel(Vector2D(10, 70), "LiberationSans-Regular", { 255,255,255,255 });
	labelPlayerHP = assets->createLabel(Vector2D(10, 100), "LiberationSans-Regular", { 255,255,255,255 });
	labelFPS = assets->createLabel(Vector2D(10, 130), "LiberationSans-Regular", { 255,255,255,255 });

	*globalbilboulga->getGameSpeed() = 1;
	globalbilboulga->setFPS(60);
}

void Game::update()
{
	if (globalbilboulga->getFPS() > 15)
	{
		*globalbilboulga->getGameSpeed() = (double)60 / (double)globalbilboulga->getFPS();
	}
	else
	{
		*globalbilboulga->getGameSpeed() = 4; //Game speed max = 4 (ou 15 FPS)
	}

	manager.refresh();
	manager.update();
	//std::cout << player->getComponent<TransformComponent>().position << std::endl;


	for (auto& p : *entitiesGroups.at(Projectiles))
	{
		for (auto& tc : Collision::getCollidersAroundEntity(p))
		{
			if (Collision::AABB(p->getComponent<ColliderComponent>(), *tc))
			{
				p->destroy();
			}
		}
	}

	for (auto& p : *entitiesGroups.at(Projectiles))
	{
		std::vector<Entity*>* targets = p->getComponent<ProjectileComponent>().getTargets();
		for (auto& t : *targets)
		{
			if (Collision::AABB(t->getComponent<ColliderComponent>().collider, p->getComponent<ColliderComponent>().collider))
			{
				p->getComponent<ProjectileComponent>().getProjectile()->targetHit(t);
			}
		}
	}

	if (manager.getGroup(Game::Players)[0]->getComponent<InputController>().pauseMode == true)
	{
		manager.getGroup(Game::Players)[0]->getComponent<InputController>().pause = true;
		while (manager.getGroup(Game::Players)[0]->getComponent<InputController>().pause == true)
		{
			manager.getGroup(Game::Players)[0]->getComponent<InputController>().update();
		}
	}

	TransformComponent playerTransform = player->getComponent<TransformComponent>();

	if (playerTransform.position.x + playerTransform.width > globalbilboulga->getCurrentRoomSize().x)
	{
		int mapNb = rand() % 4 + 1;
		
		//area1->loadMap("Arena");
		//mapNb = 4;
		switch (mapNb)
		{
		case 1:
			area1->loadMap("1");
			break;

		case 2:
			area1->loadMap("2");
			break;

		case 3:
			area1->loadMap("3");
			break;

		case 4:
			area1->loadMap("4");
			break;
		}

		globalbilboulga->setCameraW(windowSize.x);
		globalbilboulga->setCameraH(windowSize.y);
	}

	globalbilboulga->setCameraX(player->getComponent<TransformComponent>().position.x - windowSize.x / 2);
	globalbilboulga->setCameraY(player->getComponent<TransformComponent>().position.y - windowSize.y / 2);

	if (globalbilboulga->getCamera()->x < 0)
		globalbilboulga->setCameraX(0);
	if (globalbilboulga->getCamera()->y < 0)
		globalbilboulga->setCameraY(0);
	if (globalbilboulga->getCamera()->x + globalbilboulga->getCamera()->w > globalbilboulga->getCurrentRoomSize().x)
		globalbilboulga->setCameraX(globalbilboulga->getCurrentRoomSize().x - windowSize.x);
	if (globalbilboulga->getCamera()->y + globalbilboulga->getCamera()->h > globalbilboulga->getCurrentRoomSize().y)
		globalbilboulga->setCameraY(globalbilboulga->getCurrentRoomSize().y - windowSize.y);

	std::stringstream ss;
	ss << "Player position : " << player->getComponent<TransformComponent>().position;
	labelPosition->getComponent<UILabel>().setLabelText(ss.str(), "LiberationSans-Regular");

	std::stringstream ss2;
	ss2 << "Player velocity : (" << player->getComponent<TransformComponent>().velocity.x << ", " << player->getComponent<TransformComponent>().velocity.y << ")";
	labelVelocity->getComponent<UILabel>().setLabelText(ss2.str(), "LiberationSans-Regular");

	std::stringstream ss3;
	ss3 << "Player is on ground : " << (player->getComponent<TransformComponent>().onGround ? "yes":"no");
	labelOnGround->getComponent<UILabel>().setLabelText(ss3.str(), "LiberationSans-Regular");

	std::stringstream ssHP;
	ssHP << "Player HP : " << player->getComponent<RessourcesComponent>().health;
	labelPlayerHP->getComponent<UILabel>().setLabelText(ssHP.str(), "LiberationSans-Regular");

	std::stringstream ss4;
	ss4 << "FPS : " << globalbilboulga->getFPS();
	labelFPS->getComponent<UILabel>().setLabelText(ss4.str(), "LiberationSans-Regular");

	//std::cout << playerTransform.position.x << " " << playerTransform.position.y << "     " << player->getComponent<ColliderComponent>().collider.x << " " << player->getComponent<ColliderComponent>().collider.y << "     " << player->getComponent<SpriteComponent>().dest.x << " " << player->getComponent<SpriteComponent>().dest.y << std::endl;
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

	labelPosition->draw();
	labelVelocity->draw();
	labelOnGround->draw();
	labelPlayerHP->draw();
	labelFPS->draw();

	SDL_RenderPresent(globalbilboulga->getRenderer());
}

void Game::clean()
{
    globalbilboulga->clean();
    Globalbilboulga::kill();
    TTF_Quit();
	SDL_Quit();
}
