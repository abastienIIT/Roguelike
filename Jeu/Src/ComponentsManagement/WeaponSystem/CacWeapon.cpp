#include "CacWeapon.h"
#include "../Components.h"
#include "../../Common/Globalbilboulga.h"
#include "../../Collisions/Collision.h"

void CacWeapon::init(Entity* owner, std::vector<Entity*>* targets, int slot)
{
	WeaponBase::init(owner, targets, slot);

	if (asset != "")
		owner->getComponent<SpriteComponent>().setAnimatedAsset(asset, slot);

	ownerTransform = &owner->getComponent<TransformComponent>();
	ownerPos = &ownerTransform->position;

	drawAllColliders = Globalbilboulga::getInstance()->getDrawAllColliders();
}

void CacWeapon::checkHit()
{
	SDL_Rect hitbox;

	if (!ownerTransform->horizontalFlip)
		hitbox.x = ownerPos->x + hitboxDimensions.x * ownerTransform->scale;
	else
		hitbox.x = ownerPos->x - (hitboxDimensions.x + hitboxDimensions.w) * ownerTransform->scale;

	hitbox.y = ownerPos->y + hitboxDimensions.y * ownerTransform->scale;
	hitbox.w = hitboxDimensions.w * ownerTransform->scale;
	hitbox.h = hitboxDimensions.h * ownerTransform->scale;


	for (Entity* t : *targets)
	{
		if (std::find(targetsHit.begin(), targetsHit.end(), t) != targetsHit.end()) continue; //Si la cible a déjà été touchée par l'attaque, on ignore
		
		if (Collision::AABB(t->getComponent<ColliderComponent>().collider, hitbox))
		{
			t->getComponent<RessourcesComponent>().takeDamage(damages, ownerTransform->position.x, knockback);
			targetsHit.emplace_back(t);
		}
	}

	
	if (*drawAllColliders)
	{
		drawHB = true;
		hitboxToDraw = hitbox;
	}
}


void CacWeapon::drawHitbox()
{
	if (!drawHB) return;

	SDL_Rect border;

	border.x = hitboxToDraw.x - Globalbilboulga::getInstance()->getCamera()->x;
	border.y = hitboxToDraw.y - Globalbilboulga::getInstance()->getCamera()->y;
	border.w = hitboxToDraw.w;
	border.h = hitboxToDraw.h;

	TextureManager::DrawRectangle(&border);

	drawHB = false;
}

