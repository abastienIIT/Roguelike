#include "Arrow.h"

void Arrow::init(Entity* projectile, std::vector<Entity*>* targets)
{
	ProjectileBase::init(projectile, targets);

	Globalbilboulga::getInstance()->getAssetManager()->addTexture("Arrow", "assets/Projectiles/Arrow.png");

	transform->velocity = velocity;
	transform->scale = 3;
	sprite->setTex("Arrow");
	collider->tag = "Arrow";

	range = strength / 2;

	if (spriteFlipped)
	{
		sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
		collider->setCollider(SDL_Rect({ 23,0,11,3 }));
	}
	else
	{
		collider->setCollider(SDL_Rect({ 0,0,11,3 }));
	}
}

void Arrow::update()
{
	if (distance < range)
	{
		velocity.x > 0 ? distance += velocity.x : distance -= velocity.x;
	}
	else
	{
		transform->velocity.y = 1;
	}

	if (transform->position.x < 0 ||
		transform->position.y < 0 ||
		transform->position.x > Globalbilboulga::getInstance()->getCurrentRoomSize().x ||
		transform->position.y > Globalbilboulga::getInstance()->getCurrentRoomSize().y)
	{
		projectile->destroy();
	}
}
