#pragma once

#include "TileSetAsset.h"

#include "../../ComponentsManagement/Animation/Animation.h"
#include <vector>

class AnimatedTileSetAsset : public TileSetAsset
{
public:
	AnimatedTileSetAsset(std::string texturePath, std::string animationPath);

	void loadAnimations(std::string animationPath);

	int getAnimationFrameNb(int animationID) { return animations[animationID].frames; }
	int getAnimationFrameTime(int animationID) { return animations[animationID].speed; }

private:
	std::vector<Animation> animations;
};

