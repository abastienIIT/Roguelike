#include "ProjectileCreator.h"
#include "Game.h"

void ProjectileCreator::addToGroup(Entity* projectile)
{
	projectile->addGroup(Game::Projectiles);
}
