#include "AssetManager.h"
#include "ECS/Components.h"
#include <fstream>

AssetManager::AssetManager(Manager* man) : manager(man)
{}

AssetManager::~AssetManager()
{}

void AssetManager::createProjectile(Vector2D startPos, Vector2D velocity, SDL_Rect collider, int range, int speed, std::string idTex)
{
	auto& projectile(manager->addEntity());
	projectile.addComponent<TransformComponent>(startPos.x, startPos.y, 32, 32, 1);
	projectile.addComponent<SpriteComponent>(idTex);
	projectile.addComponent<ProjectileComponent>(range, speed, velocity);
	projectile.addComponent<ColliderComponent>("projectile", true, collider);
	projectile.addGroup(Game::groupProjectiles);

	projectile.getComponent<ColliderComponent>().collider.x = startPos.x + collider.x;
	projectile.getComponent<ColliderComponent>().collider.y = startPos.y + collider.y;
}

void AssetManager::createWeapon(Entity* owner, std::string weaponName, std::vector<Entity*>* targets)
{
	auto& weapon(manager->addEntity());
	weapon.addComponent<TransformComponent>(owner->getComponent<TransformComponent>().position.x, owner->getComponent<TransformComponent>().position.y, 32, 32, 3);
	weapon.addComponent<SpriteComponent>("sword");
	weapon.addComponent<ColliderComponent>("weapon", false, SDL_Rect({ 0,0,60,96 }));
	weapon.addComponent<WeaponComponent>(owner, targets);
	weapon.addGroup(Game::groupWeapon);

	owner->getComponent<ActionsComponent>().setWeapon(&weapon.getComponent<WeaponComponent>());
}

void AssetManager::createEnemies(int id, Vector2D pos)
{
	auto& enemie(manager->addEntity());

	Entity* player = manager->getGroup(Game::groupPlayers)[0];

	switch (id)
	{
	case 0:
		enemie.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 3);
		enemie.addComponent<SpriteComponent>("enemie", true);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ 6,9,14,23 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player, 1);
		break;

	case 1:
		enemie.addComponent<TransformComponent>(pos.x, pos.y, 32, 64, 3);
		enemie.addComponent<SpriteComponent>("giant", true);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ 8,6,14,58 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player, 2);
		break;

	default:
		break;
	}

	enemie.addGroup(Game::groupEnemies);
}

void AssetManager::createPlayer()
{
	auto& player(manager->addEntity());

	player.addComponent<TransformComponent>(0, 863, 32, 32, 3);
	player.addComponent<SpriteComponent>("player", true);
	player.addComponent<ColliderComponent>("player", true, SDL_Rect({ 5,2,19,30 }));
	player.addComponent<ActionsComponent>();
	player.addComponent<InputController>();
	player.addGroup(Game::groupPlayers);

	createWeapon(&player, "", &manager->getGroup(Game::groupEnemies));
}

Entity* AssetManager::createLabel(Vector2D pos, std::string policeName, SDL_Color color)
{
	auto& label(manager->addEntity());

	label.addComponent<UILabel>(pos, "Hello", policeName, color);

	return &label;
}

void AssetManager::addTexture(std::string id, const char* path)
{
	textures.emplace(id, TextureManager::LoadTexture(path));
}

void AssetManager::addAnimatedTexture(std::string id, const char* path, const char* pathAnim)
{
	textures.emplace(id, TextureManager::LoadTexture(path));

	char c;
	std::string num = "";
	std::string IDAnim = "";
	int index;
	int frames;
	int speed;
	std::map<std::string, Animation> ensemble;
	std::fstream animationInfos;
	animationInfos.open(pathAnim);

	animationInfos.get(c);
	while (c != '\n')
	{
		animationInfos.get(c);
	}

	animationInfos.get(c);
	while (c != '.')
	{
		IDAnim = "";
		while (c != ',')
		{
			IDAnim += c;
			animationInfos.get(c);
		}
		animationInfos.get(c);

		while (c != ',')
		{
			num += c;
			animationInfos.get(c);
		}
		index = stoi(num);

		num = "";
		animationInfos.get(c);

		while (c != ',')
		{
			num += c;
			animationInfos.get(c);
		}
		frames = stoi(num);

		num = "";
		animationInfos.get(c);

		while (c != '\n')
		{
			num += c;
			animationInfos.get(c);
		}
		speed = stoi(num);

		num = "";
		animationInfos.get(c);

		ensemble.emplace(IDAnim, Animation(index, frames, speed));
	}

	animations.emplace(id, ensemble);
}

SDL_Texture* AssetManager::getTexture(std::string id)
{
	return textures[id];
}

std::map<std::string, Animation> AssetManager::getAnim(std::string id)
{
	return animations[id];
}

void AssetManager::addFont(std::string id, std::string path, int fontSize)
{
	fonts.emplace(id, TTF_OpenFont(path.c_str(), fontSize));
}

TTF_Font* AssetManager::getFont(std::string id)
{
	return fonts[id];
}
