#include "Fireball.h"
#include "../../Components.h"
#include "../../../Common/Globalbilboulga.h"

void Fireball::init(Entity* projectile, std::vector<Entity*>* targets)
{
	ProjectileBase::init(projectile, targets);

	transform->velocity = velocity;

	sprite->setAsset("Fireball");
	sprite->setCurrentTexture(0);

	collider->tag = "Fireball";
	collider->setCollider(SDL_Rect({ 0,0,32,32 }));
}

void Fireball::update()
{
	distance += velocity.x;

	if (distance > range ||
		distance < range * -1 ||
		transform->position.x < 0 ||
		transform->position.y < 0 ||
		transform->position.x > Globalbilboulga::getInstance()->getCurrentRoomSize().x ||
		transform->position.y > Globalbilboulga::getInstance()->getCurrentRoomSize().y)
	{
		projectile->destroy();
	}
	
}
