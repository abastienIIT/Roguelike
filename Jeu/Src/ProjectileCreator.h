#pragma once

#include "Common/Types/Vector2D.h"
#include "ComponentsManagement/ECS.h"
#include "ComponentsManagement/ProjectileSystem/Projectiles.h"
#include "Game.h"
#include <vector>
#include <iostream>

class ProjectileCreator
{
public:
	ProjectileCreator(Manager* manager) : manager(manager) {};

	template<typename projType, typename... TArgs>
	void createProjectile(Vector2D startPos, std::vector<Entity*>* targets, TArgs&&... mArgs)
	{
		auto& projectile(manager->addEntity());
		projectile.addComponent<TransformComponent>(startPos.x, startPos.y, 32, 32, 1);
		projectile.addComponent<SpriteComponent>();
		projectile.addComponent<ColliderComponent>("projectile", true);
		projectile.addComponent<ProjectileComponent>(targets);
		projectile.getComponent<ProjectileComponent>().setProjectile<Fireball>(std::forward<TArgs>(mArgs)...);
		projectile.addGroup(Game::Projectiles);

		projectile.getComponent<ColliderComponent>().collider.x = startPos.x + projectile.getComponent<ColliderComponent>().collider.x;
		projectile.getComponent<ColliderComponent>().collider.y = startPos.y + projectile.getComponent<ColliderComponent>().collider.y;
	}

private:
	Manager* manager;
};