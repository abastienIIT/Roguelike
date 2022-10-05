#include "TrapCreator.h"
#include "Game.h"
#include "ComponentsManagement/Unique/TransformComponent.h"
#include "ComponentsManagement/Unique/SpriteComponent.h"
#include "ComponentsManagement/Unique/ColliderComponent.h"
#include "ComponentsManagement/TrapSystem/Traps.h"

void TrapCreator::initTrapMap(Vector2D roomSize)
{
	trapMap.clear();

	std::pair<int, Entity*> tile = std::make_pair(-1, nullptr);
	std::vector<std::pair<int, Entity*>> column(roomSize.y, tile);
	
	for (int i = 0; i < roomSize.x; i++)
		trapMap.emplace_back(column);
}

void TrapCreator::createTrap(int id, Vector2D pos)
{
	Entity& trap(manager->addEntity());
	Entity* trapMaster = nullptr;
	
	Entity* player = manager->getGroup(Game::Players)[0];

	int tileSize = Globalbilboulga::getInstance()->getCurrentTileSize();

	switch (id)
	{
	case 0: //Spike trap left
		trapMap.at(pos.x / tileSize).at(pos.y / tileSize) = std::make_pair(0, &trap);

		trap.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, tileSize / 32);
		trap.addComponent<SpriteComponent>("spikeTrapLeft", true);
		trap.addComponent<ColliderComponent>("trap", false, SDL_Rect{ pos.x + 3 * tileSize / 32, pos.y + 2 * tileSize / 3 , tileSize - 3 * tileSize / 32, tileSize / 3 });
		trap.addComponent<TrapComponent>(player);
		trap.getComponent<TrapComponent>().setTrap<SpikeTrap>();
		break;

	case 1: //Spike trap center
		if (trapMap.at(static_cast<size_t>(pos.x / tileSize) - 1).at(pos.y / tileSize).first != 0)
			break;
		
		trapMaster = trapMap.at(static_cast<size_t>(pos.x / tileSize) - 1).at(pos.y / tileSize).second;

		trapMap.at(pos.x / 64).at(pos.y / 64) = std::make_pair(0, trapMaster);

		trap.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
		trap.addComponent<SpriteComponent>("spikeTrapCenter", true);

		trapMaster->getComponent<TrapComponent>().getTrap<SpikeTrap>().growTrap(&trap, tileSize);
		
		break;

	case 2: //Spike trap right
		if (trapMap.at(static_cast<size_t>(pos.x / tileSize) - 1).at(pos.y / tileSize).first != 0)
			break;

		trapMaster = trapMap.at(static_cast<size_t>(pos.x / tileSize) - 1).at(pos.y / tileSize).second;

		trapMap.at(pos.x / 64).at(pos.y / 64) = std::make_pair(0, trapMaster);

		trap.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 2);
		trap.addComponent<SpriteComponent>("spikeTrapRight", true);

		trapMaster->getComponent<TrapComponent>().getTrap<SpikeTrap>().growTrap(&trap, tileSize - 3 * tileSize / 32);

		break;

	default :
		break;
	}

	trap.addGroup(Game::Traps);
}
