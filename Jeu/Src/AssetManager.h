#pragma once

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>


#include "Common/Types/Vector2D.h"
#include "ComponentsManagement/ECS.h"
#include "ComponentsManagement/Animation/Animation.h"

class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	Entity* createLabel(Vector2D pos, std::string policeName, SDL_Color color);


	//texture management
	void addTexture(std::string id, const char* path);
	void addAnimatedTexture(std::string id, const char* path, const char* pathAnim);
	SDL_Texture* getTexture(std::string id);
	std::vector<SDL_Texture*>* getnewTexture(std::string id);
	std::map<std::string, Animation> getAnim(std::string id);

	void addFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);

private:
	Manager* manager;

	std::map<std::string, std::vector<SDL_Texture*>*> newtextures;
	std::map<std::string, SDL_Texture*> textures;
	std::map<std::string, std::map<std::string, Animation>> animations;

	std::map<std::string, TTF_Font*> fonts;
};
