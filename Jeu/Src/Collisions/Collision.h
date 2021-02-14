#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <vector>
#include <cmath>

#include "../ComponentsManagement/Unique/ColliderComponent.h"
#include "../ComponentsManagement/ECS.h"
#include "../Common/Types/Vector2D.h"

#define CUTE_C2_IMPLEMENTATION
#include "cute_c2.h"

class ColliderComponent;

class Collision
{
public:
	static bool AABB(const SDL_Rect& recA, const SDL_Rect& recB);
	static bool AABB(const ColliderComponent& colA, const ColliderComponent& colB);

	static void resolveCollisions(Entity *players, std::vector<Entity *> colliders);
};

