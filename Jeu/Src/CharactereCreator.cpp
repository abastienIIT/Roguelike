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
		enemie.addComponent<SpriteComponent>("enemie", true);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ 6,9,14,23 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player);
		enemie.getComponent<IAComponent>().setIA<SimpleFollowFarAndShoot>();
		enemie.addComponent<RessourcesComponent>();
		break;

	case 1:
		enemie.addComponent<TransformComponent>(pos.x, pos.y, 32, 64, 3, true, true, true);
		enemie.getComponent<TransformComponent>().truePosition.y -= 32 * enemie.getComponent<TransformComponent>().scale;
		enemie.addComponent<SpriteComponent>("giant", true);
		enemie.addComponent<ColliderComponent>("enemie", true, SDL_Rect({ 8,6,14,58 }));
		enemie.addComponent<ActionsComponent>();
		enemie.addComponent<IAComponent>(player);
		enemie.getComponent<IAComponent>().setIA<SimpleFollow>();
		enemie.addComponent<RessourcesComponent>();
		break;

	default:
		break;
	}

	enemie.addGroup(Game::Enemies);
}

void CharactereCreator::createPlayer()
{
	auto& player(manager->addEntity());

	player.addComponent<TransformComponent>(0, 863, 32, 32, 3, true, true, true);
	player.addComponent<SpriteComponent>("playerBase", true);
	player.addComponent<ColliderComponent>("player", true, SDL_Rect({ 5,2,19,30 }));
	player.addComponent<ActionsComponent>();
	player.addComponent<InputController>();
	player.addComponent<RessourcesComponent>();
	player.addComponent<WeaponComponent>(&manager->getGroup(Game::Enemies));
	player.getComponent<WeaponComponent>().setWeapon<BasicSword>();
	player.getComponent<WeaponComponent>().setWeapon<BasicBow>(true);
	player.addGroup(Game::Players);

	player.getComponent<ColliderComponent>().drawCollider = true;
}