#include "Arrow.h"
#include "../../Unique/RessourcesComponent.h"
#include "../../../Common/Globalbilboulga.h"
#include <math.h>

void Arrow::init(Entity* projectile, std::vector<Entity*>* targets)
{
	ProjectileBase::init(projectile, targets);

	transform->velocity = initialVelocity;
	transform->gravityCoef = 0.18;
	transform->gravity_pull_limit = 100; // no limit
	transform->scale = 3;
	sprite->setAsset("Arrow");
	sprite->setCurrentTexture(0);
	collider->tag = "Arrow";

	SDL_Point center;
	center.x = 5 * transform->scale;
	center.y = 1 * transform->scale;
	transform->rotationCenter = center;

	range = static_cast<int>(strength / 1.2);

	collider->setCollider(SDL_Rect({0, 0, 11, 3}));
}

void Arrow::update()
{
	if (transform->velocity.x > 0)
		distance += transform->velocity.x;
	else
		distance -= transform->velocity.x;

	if (!transform->applyGravity && distance >= range)
		transform->applyGravity = true;

	if (transform->position.x < 0 ||
		transform->position.y < 0 ||
		transform->position.x > Globalbilboulga::getInstance()->getCurrentRoomSize().x ||
		transform->position.y > Globalbilboulga::getInstance()->getCurrentRoomSize().y)
	{
		projectile->destroy();
	}

	// Manage arrow sprite rotation
	double r = atan(transform->velocity.y / transform->velocity.x) * 180 / M_PI;
	transform->rotation = r + 180.0 * spriteFlipped;
}

void Arrow::targetHit(Entity* target)
{
	if (target->getComponent<RessourcesComponent>().takeDamage(damages, transform->position.x, knockback))
		this->projectileComponent->entity->destroy();
}
