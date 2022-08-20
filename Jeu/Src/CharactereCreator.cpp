#include "CharactereCreator.h"
#include "Game.h"
#include "ComponentsManagement/ActionsComponent/ActionsComponent.h"
#include "ComponentsManagement/IASystem/IAs.h"
#include "ComponentsManagement/Unique/InputController.h"
#include "ComponentsManagement/WeaponSystem/Weapons.h"
#include "ComponentsManagement/Unique/RessourcesComponent.h"

void CharactereCreator::createEnemies(int id, Vector2D pos)
{
	auto& enemie(manager->addEntity());

	Entity* player = manager->getGroup(Game::Players)[0];

	switch (id)
	{
	case 0:
		enemie.addComponent<TransformComponent>(pos.x, pos.y, 32, 32, 3, true, true, true);
		enemie.getComponent<TransformComponent>().truePosition.x += 32;
		enemie.addComponent<SpriteComponent>("enemie", true, 13, 18);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ -7,-9,14,23 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player);
		enemie.getComponent<IAComponent>().setIA<SimpleFollowFarAndShoot>();
		enemie.addComponent<RessourcesComponent>(120);
		break;

	case 1:
		enemie.addComponent<TransformComponent>(pos.x, pos.y, 32, 64, 3, true, true, true);
		enemie.getComponent<TransformComponent>().truePosition.x += 32;
		enemie.addComponent<SpriteComponent>("giant", true, 15, 27);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ -8,-22,14,58 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player);
		enemie.getComponent<IAComponent>().setIA<SimpleFollow>();
		enemie.addComponent<RessourcesComponent>(120);
		break;

	case 2:
		enemie.addComponent<TransformComponent>(pos.x, pos.y, 128, 128, 3, true, true, true);
		enemie.getComponent<TransformComponent>().truePosition.x += 32;
		//enemie.getComponent<TransformComponent>().truePosition.y -= (90 + 64);
		enemie.addComponent<SpriteComponent>("BigKnight", true, 66, 90);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ -16,-21,28,58 }));
		//enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ 17,3,30,60 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player);
		enemie.getComponent<IAComponent>().setIA<BigKnightIA>();
		enemie.addComponent<RessourcesComponent>(120);
		enemie.addComponent<WeaponComponent>(&manager->getGroup(Game::Players));
		enemie.getComponent<WeaponComponent>().setWeapon<BKWeapon>(false);
		enemie.getComponent<WeaponComponent>().setWeapon<BKWeapon>(true);

	default:
		break;
	}

	enemie.addGroup(Game::Enemies);
}

void CharactereCreator::createPlayer()
{
	auto& player(manager->addEntity());

	player.addComponent<TransformComponent>(0, 863, 32, 32, 3, true, true, true);
	player.addComponent<SpriteComponent>("playerBase", true, 14, 14);
	player.addComponent<ColliderComponent>("player", true, SDL_Rect({ -9,-13,19,31 }));
	player.addComponent<ActionsComponent>();
	player.addComponent<InputController>();
	player.addComponent<RessourcesComponent>();
	player.addComponent<WeaponComponent>(&manager->getGroup(Game::Enemies));
	player.getComponent<WeaponComponent>().setWeapon<BasicSword>(false);
	player.getComponent<WeaponComponent>().setWeapon<BasicBow>(true);
	player.addGroup(Game::Players);

	player.getComponent<ColliderComponent>().drawCollider = true;
}