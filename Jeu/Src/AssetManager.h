#pragma once

#include <map>
#include <string>
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/ECS.h"
#include <SDL2/SDL_ttf.h>
#include "ECS/Animation.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	//gameObjects
	void createProjectile(Vector2D startPos, Vector2D velocity, SDL_Rect collider, int range, int speed, std::string idTex);
	void createWeapon(Entity* owner, std::string weapon, std::vector<Entity*>* targets);

	void createEnemies(int id, Vector2D pos);


	//texture management
	void addTexture(std::string id, const char* path);
	void addAnimatedTexture(std::string id, const char* path, const char* pathAnim);
	SDL_Texture* getTexture(std::string id);
	std::map<std::string, Animation> getAnim(std::string id);

	void addFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);

private:
	Manager* manager;

	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, std::map<std::string, Animation>> animations;

	std::map<std::string, TTF_Font*> fonts;
};
