#pragma once

#include <map>
#include <string>
#include <SDL2/SDL_ttf.h>

#include "Common/Types/Vector2D.h"
#include "ComponentsManagement/ECS.h"
#include "Common/Types/Assets/Asset.h"
#include "Common/Types/Assets/AnimatedAsset.h"
#include "Common/Types/Assets/TileSetAsset.h"
#include "Common/Types/Assets/AnimatedTileSetAsset.h"


class AssetManager
{
public:
	AssetManager(Manager* man);
	~AssetManager();

	//Texture management
	void addAsset(std::string id, Asset* asset);
	void addAnimatedAsset(std::string id, AnimatedAsset* asset);
	void addTileSetAsset(std::string id, TileSetAsset* asset);
	void addAnimatedTileSetAsset(std::string id, AnimatedTileSetAsset* asset);
	Asset* getAsset(std::string id);
	AnimatedAsset* getAnimatedAsset(std::string id);
	TileSetAsset* getTileSetAsset(std::string id);
	AnimatedTileSetAsset* getAnimatedTileSetAsset(std::string id);

	//Font Management
	Entity* createLabel(Vector2D pos, std::string policeName, SDL_Color color);
	void addFont(std::string id, std::string path, int fontSize);
	TTF_Font* getFont(std::string id);

private:
	Manager* manager;

	std::map<std::string, Asset*> assets;
	std::map<std::string, AnimatedAsset*> animatedAssets;
	std::map<std::string, TileSetAsset*> tileSetAssets;
	std::map<std::string, AnimatedTileSetAsset*> animatedTileSetAssets;

	std::map<std::string, TTF_Font*> fonts;
};
