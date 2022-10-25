#include "Collision.h"

#include <vector>
#include <cmath>

#include "../ComponentsManagement/Unique/TileComponent.h"
#include "../ComponentsManagement/Unique/ColliderComponent.h"
#include "../ComponentsManagement/Unique/SpriteComponent.h"
#include "../Common/Types/DoubleVector.h"
#include "../Common/Globalbilboulga.h"

#define CUTE_C2_IMPLEMENTATION
#include "cute_c2.h"


bool Collision::AABB(const SDL_Rect& recA, const SDL_Rect& recB)
{
	if (recB.w == -51) std::cout << recA.x << " " << recA.y << " " << recA.w << " " << recA.h << "\n" << recB.x << " " << recB.y << " " << recB.w << " " << recB.h << "\n" << std::endl;
	if (
		recA.x + recA.w >= recB.x &&
		recB.x + recB.w >= recA.x &&
		recA.y + recA.h >= recB.y &&
		recB.y + recB.h >= recA.y
		)
	{
		return true;
	}

	return false;
}

bool Collision::AABB(const ColliderComponent& colA, const ColliderComponent& colB)
{
	if (colA.tag != colB.tag)
	{
		if (AABB(colA.collider, colB.collider))
		{
			//std::cout << colA.tag << " hit " << colB.tag << std::endl;
			return true;
		}
	}
	return false;
}

std::vector<ColliderComponent*> Collision::getCollidersAroundEntity(Entity* entity)
{
	SDL_Rect* collider = &entity->getComponent<ColliderComponent>().collider;
	int sizeTile = Globalbilboulga::getInstance()->getCurrentTileSize();
	Vector2D roomSize = Globalbilboulga::getInstance()->getCurrentRoomSize() / sizeTile;

	Uint64 firstTileX = collider->x / sizeTile; //Tile en haut à gauche de l'entité
	Uint64 firstTileY = collider->y / sizeTile;

	int horiLong = collider->w / sizeTile + 2; //Nombre de tiles maximum pris par l'entité horizontalement
	int vertLong = collider->h / sizeTile + 2;

	std::vector<std::vector<ColliderComponent*>>* mapCollider = Globalbilboulga::getInstance()->getCurrentMapColliders();

	std::vector<ColliderComponent*> collidersAroundEntity;

	for (int x = 0; x < horiLong && firstTileX + x < roomSize.x; x++)
	{
		for (int y = 0; y < vertLong && firstTileY + y < roomSize.y; y++)
		{
			if (mapCollider->at(firstTileY + y).at(firstTileX + x) != nullptr)
			{
				Entity* currentTile = mapCollider->at(firstTileY + y).at(firstTileX + x)->entity;

				collidersAroundEntity.emplace_back(&currentTile->getComponent<ColliderComponent>());
				currentTile->getComponent<TileComponent>().drawOutline = true;
			}
		}
	}

	return collidersAroundEntity;
}

void Collision::resolveCollisions(Entity* player)
{
	SDL_Rect* playerRect = &player->getComponent<ColliderComponent>().collider;
	SDL_Rect* playerColSrc = &player->getComponent<ColliderComponent>().colliderSrc;
	SDL_Rect tcCollider;

	std::vector<ColliderComponent*> collidersAroundEntity;

	Vector2D startPos = player->getComponent<TransformComponent>().position;

	TransformComponent* transform = &player->getComponent<TransformComponent>();

	Vector2D deplacement;

	c2AABB c2PlayerRect;
	c2AABB c2tcCollider;

	c2Manifold manifold;
	std::vector<std::pair<int,c2v>> manifolds;

	bool hasCollided = true;

	float longestManifold = 0;
	int longestManifoldIndex = 0;
	int currentIndex = 0;

	bool manifoldConflictX = false;
	bool manifoldConflictY = false;

	while (hasCollided)
	{
		collidersAroundEntity = getCollidersAroundEntity(player);

		hasCollided = false;

		longestManifold = 0;
		longestManifoldIndex = 0;
		currentIndex = 0;

		manifoldConflictX = false;
		manifoldConflictY = false;

		for (auto& tc : collidersAroundEntity)
		{
			tcCollider = tc->collider;

			if (Collision::AABB(*playerRect, tcCollider))
			{
				hasCollided = true;

				c2PlayerRect = { c2v{static_cast<float>(playerRect->x), static_cast<float>(playerRect->y)},
					c2v{static_cast<float>(playerRect->x + playerRect->w), static_cast<float>(playerRect->y + playerRect->h)} };
				c2tcCollider = { c2v{static_cast<float>(tcCollider.x), static_cast<float>(tcCollider.y)},
					c2v{static_cast<float>(tcCollider.x + tcCollider.w), static_cast<float>(tcCollider.y + tcCollider.h)} };

				c2AABBtoAABBManifold(c2PlayerRect, c2tcCollider, &manifold);

				if (manifold.depths[0] > longestManifold)
				{
					longestManifoldIndex = currentIndex;
					longestManifold = manifold.depths[0];
				}

				for (auto& m : manifolds)
				{
					if (manifold.n.x != 0 && m.second.x + manifold.n.x == 0) manifoldConflictX = true;
					if (manifold.n.y != 0 && m.second.y + manifold.n.y == 0) manifoldConflictY = true;
				}

				manifolds.push_back(std::make_pair(static_cast<int>(manifold.depths[0]) + 1, manifold.n));

				currentIndex++;
			}
		}

		if (hasCollided)
		{
			if (longestManifoldIndex == 0 && manifolds[0].second.y == -1 && manifolds.size() > 1 && manifolds[1].second.x != 0)
			{
				longestManifoldIndex = 1;
			}

			if (manifolds[longestManifoldIndex].second.x != 0 && !manifoldConflictX)
			{
				playerRect->x -= static_cast<int>(manifolds[longestManifoldIndex].second.x) * manifolds[longestManifoldIndex].first;
			}
			else if (manifolds[longestManifoldIndex].second.y != 0 && !manifoldConflictY)
			{
				playerRect->y -= static_cast<int>(manifolds[longestManifoldIndex].second.y) * manifolds[longestManifoldIndex].first;
			}
			else
			{
				deplacement = transform->position;
				deplacement -= transform->previousPos;

				if (manifoldConflictX)
				{
					if (deplacement.y > 0) playerRect->y -= manifolds[longestManifoldIndex].first;
					else playerRect->y += manifolds[longestManifoldIndex].first;
				}
				else if (manifoldConflictY)
				{
					if (deplacement.x > 0) playerRect->x -= manifolds[longestManifoldIndex].first;
					else playerRect->x += manifolds[longestManifoldIndex].first;
				}
			}

			manifolds.clear();

			transform->position.x = playerRect->x - (playerColSrc->x - transform->horizontalFlip * (playerColSrc->w + 2 * playerColSrc->x)) * transform->scale;
			transform->position.y = playerRect->y - playerColSrc->y * transform->scale;

			if (transform->position.x != startPos.x)
			{
				transform->truePosition.x = transform->position.x;
			}
			else
			{
				transform->position.x = (int)transform->truePosition.x;
			}

			if (transform->position.y != startPos.y)
			{
				transform->truePosition.y = transform->position.y;
			}
			else
			{
				transform->position.y = (int)transform->truePosition.y;
			}
		}
	}
	//player->getComponent<SpriteComponent>().update();
}

/*
void Collision::resolveCollisions(Entity* player, std::vector<Entity*> terrainColliders)
{
	Vector2D deplacement = player->getComponent<TransformComponent>().position;
	deplacement -= player->getComponent<TransformComponent>().previousPos;

	SDL_Rect playerCol;
	SDL_Rect currentTile;

	bool playerSidesCollided[4]; // Bas, Droite, Gauche, Haut
	SDL_Rect playerSides[4]; // Bas, Droite, Gauche, Haut

	int nextPosMax[4]; //Plus bas, Plus à droite, Plus à gauche, Plus haut

	int commonLength[4]; // Bas, Droite, Gauche, Haut

	bool hasCollided = true;

	while (hasCollided)
	{
		hasCollided = false;

		playerCol = player->getComponent<ColliderComponent>().collider;

		nextPosMax[0] = playerCol.y - deplacement.y;
		nextPosMax[1] = playerCol.x - deplacement.x;
		nextPosMax[2] = playerCol.x - deplacement.x;
		nextPosMax[3] = playerCol.y - deplacement.y;

		commonLength[0] = 0;
		commonLength[1] = 0;
		commonLength[2] = 0;
		commonLength[3] = 0;

		for (int i = 0; i < 4; i++) playerSidesCollided[i] = false;

		playerSides[0] = { playerCol.x, playerCol.y + playerCol.h, playerCol.w, 0 };
		playerSides[1] = { playerCol.x + playerCol.w, playerCol.y, 0, playerCol.h };
		playerSides[2] = { playerCol.x, playerCol.y, 0, playerCol.h };
		playerSides[3] = { playerCol.x, playerCol.y, playerCol.w, 0 };

		for (auto& tc : terrainColliders)
		{
			currentTile = tc->getComponent<ColliderComponent>().collider;

			if (Collision::AABB(playerCol, currentTile))
			{
				hasCollided = true;

				if (Collision::AABB(playerSides[0], currentTile))
				{
					playerSidesCollided[0] = true;

					if (nextPosMax[0] < currentTile.y - 1 - playerCol.h) nextPosMax[0] = currentTile.y - 1 - playerCol.h;

					if (playerCol.x < currentTile.x && playerCol.x + playerCol.w < currentTile.x + currentTile.w) commonLength[0] += playerCol.x + playerCol.w - currentTile.x;
					else if (playerCol.x > currentTile.x && playerCol.x + playerCol.w > currentTile.x + currentTile.w) commonLength[0] += currentTile.x + currentTile.w - playerCol.x;
					else if (playerCol.x < currentTile.x && playerCol.x + playerCol.w > currentTile.x + currentTile.w) commonLength[0] += currentTile.w;
					else if (playerCol.x > currentTile.x && playerCol.x + playerCol.w < currentTile.x + currentTile.w) commonLength[0] += playerCol.w;
				}

				if (Collision::AABB(playerSides[1], currentTile))
				{
					playerSidesCollided[1] = true;

					if (nextPosMax[1] < currentTile.x - 1 - playerCol.w) nextPosMax[1] = currentTile.x - 1 - playerCol.w;

					if (playerCol.y < currentTile.y && playerCol.y + playerCol.h < currentTile.y + currentTile.h) commonLength[1] += playerCol.y + playerCol.h - currentTile.y;
					else if (playerCol.y > currentTile.y && playerCol.y + playerCol.h > currentTile.y + currentTile.h) commonLength[1] += currentTile.y + currentTile.h - playerCol.y;
					else if (playerCol.y < currentTile.y && playerCol.y + playerCol.h > currentTile.y + currentTile.h) commonLength[1] += currentTile.h;
					else if (playerCol.y > currentTile.y && playerCol.y + playerCol.h < currentTile.y + currentTile.h) commonLength[1] += playerCol.h;
				}

				if (Collision::AABB(playerSides[2], currentTile))
				{
					playerSidesCollided[2] = true;

					if (nextPosMax[2] > currentTile.x + currentTile.w + 1) nextPosMax[2] = currentTile.x + currentTile.w + 1;

					if (playerCol.y < currentTile.y && playerCol.y + playerCol.h < currentTile.y + currentTile.h) commonLength[2] += playerCol.y + playerCol.h - currentTile.y;
					else if (playerCol.y > currentTile.y && playerCol.y + playerCol.h > currentTile.y + currentTile.h) commonLength[2] += currentTile.y + currentTile.h - playerCol.y;
					else if (playerCol.y < currentTile.y && playerCol.y + playerCol.h > currentTile.y + currentTile.h) commonLength[2] += currentTile.h;
					else if (playerCol.y > currentTile.y && playerCol.y + playerCol.h < currentTile.y + currentTile.h) commonLength[2] += playerCol.h;
				}

				if (Collision::AABB(playerSides[3], currentTile))
				{
					playerSidesCollided[3] = true;

					if (nextPosMax[3] > currentTile.y + currentTile.h + 1) nextPosMax[3] = currentTile.y + currentTile.h + 1;

					if (playerCol.x < currentTile.x && playerCol.x + playerCol.w < currentTile.x + currentTile.w) commonLength[3] += playerCol.x + playerCol.w - currentTile.x;
					else if (playerCol.x > currentTile.x && playerCol.x + playerCol.w > currentTile.x + currentTile.w) commonLength[3] += currentTile.x + currentTile.w - playerCol.x;
					else if (playerCol.x < currentTile.x && playerCol.x + playerCol.w > currentTile.x + currentTile.w) commonLength[3] += currentTile.w;
					else if (playerCol.x > currentTile.x && playerCol.x + playerCol.w < currentTile.x + currentTile.w) commonLength[3] += playerCol.w;
				}
			}
		}

		int nbOfSidesCollided = int(playerSidesCollided[0]) + int(playerSidesCollided[1]) + int(playerSidesCollided[2]) + int(playerSidesCollided[3]);

		switch (nbOfSidesCollided)
		{
		case 1:
			if (playerSidesCollided[0]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[0];
			else if (playerSidesCollided[1]) player->getComponent<ColliderComponent>().collider.x = nextPosMax[1];
			else if (playerSidesCollided[2]) player->getComponent<ColliderComponent>().collider.x = nextPosMax[2];
			else if (playerSidesCollided[3]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[3];
			break;

		case 2:
			if (deplacement.y > 0 && playerSidesCollided[0] && deplacement.x > 0 && playerSidesCollided[1])
			{
				if (commonLength[1] <= commonLength[0]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[0];
				else player->getComponent<ColliderComponent>().collider.x = nextPosMax[1];
			}
			else if (deplacement.y > 0 && playerSidesCollided[0] && deplacement.x < 0 && playerSidesCollided[2])
			{
				if (commonLength[2] <= commonLength[0]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[0];
				else player->getComponent<ColliderComponent>().collider.x = nextPosMax[2];
			}
			else if (deplacement.y < 0 && playerSidesCollided[3] && deplacement.x > 0 && playerSidesCollided[1])
			{
				if (commonLength[1] <= commonLength[3]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[3];
				else player->getComponent<ColliderComponent>().collider.x = nextPosMax[1];
			}
			else if (deplacement.y < 0 && playerSidesCollided[3] && deplacement.x < 0 && playerSidesCollided[2])
			{
				if (commonLength[2] <= commonLength[3]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[3];
				else player->getComponent<ColliderComponent>().collider.x = nextPosMax[2];
			}
			else
			{
				if (deplacement.y > 0 && playerSidesCollided[0]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[0];
				else if (deplacement.x > 0 && playerSidesCollided[1]) player->getComponent<ColliderComponent>().collider.x = nextPosMax[1];
				else if (deplacement.x < 0 && playerSidesCollided[2]) player->getComponent<ColliderComponent>().collider.x = nextPosMax[2];
				else if (deplacement.y < 0 && playerSidesCollided[3]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[3];
			}
			break;

		case 3:
			if (!playerSidesCollided[3]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[0];
			else if (!playerSidesCollided[2]) player->getComponent<ColliderComponent>().collider.x = nextPosMax[1];
			else if (!playerSidesCollided[1]) player->getComponent<ColliderComponent>().collider.x = nextPosMax[2];
			else if (!playerSidesCollided[0]) player->getComponent<ColliderComponent>().collider.y = nextPosMax[3];
			break;

		case 4:
			if (deplacement.y > 0) player->getComponent<ColliderComponent>().collider.y = nextPosMax[0];
			if (deplacement.x > 0) player->getComponent<ColliderComponent>().collider.x = nextPosMax[1];
			if (deplacement.x < 0) player->getComponent<ColliderComponent>().collider.x = nextPosMax[2];
			if (deplacement.y < 0) player->getComponent<ColliderComponent>().collider.y = nextPosMax[3];
			break;

		default:
			break;
		}
	}

	playerCol = player->getComponent<ColliderComponent>().collider;
	SDL_Rect playerColSrc = player->getComponent<ColliderComponent>().colliderSrc;

	player->getComponent<TransformComponent>().position.x = playerCol.x - playerColSrc.x * player->getComponent<TransformComponent>().scale;
	player->getComponent<TransformComponent>().position.y = playerCol.y - playerColSrc.y * player->getComponent<TransformComponent>().scale;
}
*/
