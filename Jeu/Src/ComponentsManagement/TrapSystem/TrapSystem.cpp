#include "TrapSystem.h"
#include "../Unique/ColliderComponent.h"
#include "../Unique/RessourcesComponent.h"
#include "../../Collisions/Collision.h"

bool TrapBase::damageTargetsOnTrap(SDL_Rect* trapHitbox)
{
	bool targetOnTrap = false;

	for (Entity* t : targets)
	{
		if (Collision::AABB(t->getComponent<ColliderComponent>().collider, *trapHitbox))
		{
			if (std::find(targetsHit.begin(), targetsHit.end(), t) == targetsHit.end()) //Si la target n'a pas encore pris les dégats
			{
				t->getComponent<RessourcesComponent>().takeDamage(damages, owner->getComponent<TransformComponent>().position.x, knockback);
				targetsHit.emplace_back(t);
			}

			targetOnTrap = true;
		}
	}

    return targetOnTrap;
}
