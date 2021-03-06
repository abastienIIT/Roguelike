#pragma once

#include <vector>
#include <SDL2/SDL.h>

#include "../ComponentsManagement/ECS.h"
#include "../Common/Types/Vector2D.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

	static void resolveCollisions(Entity *players, std::vector<Entity *> colliders);
};

