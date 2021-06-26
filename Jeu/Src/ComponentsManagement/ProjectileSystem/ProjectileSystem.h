#pragma once
#include <map>
#include "../ECS.h"
#include "../Animation/Animation.h"
#include "../Unique/TransformComponent.h"
#include "../Unique/SpriteComponent.h"
#include "../Unique/ColliderComponent.h"

class ProjectileBase;
class ProjectileComponent;

class ProjectileBase
{
public:

	ProjectileComponent* projectileComponent;

	ProjectileBase() {}
	virtual ~ProjectileBase() {}

	void init(Entity* projectile, std::vector<Entity*>* targets)
	{
		this->projectile = projectile;
		this->targets = targets;

		transform = &projectile->getComponent<TransformComponent>();
		sprite = &projectile->getComponent<SpriteComponent>();
		collider = &projectile->getComponent<ColliderComponent>();
	}

	virtual void update() {}

protected:
	Entity* projectile = nullptr;
	std::vector<Entity*>* targets;
	std::map<std::string, Animation> animations;

	TransformComponent* transform = nullptr;
	SpriteComponent* sprite = nullptr;
	ColliderComponent* collider = nullptr;
};

class ProjectileComponent : public Component
{
public:
	ProjectileComponent(Entity* target) { targets->emplace_back(target); }
	ProjectileComponent(std::vector<Entity*>* targets) : targets(targets) {}
	~ProjectileComponent() {}

	void init() override {}
	void update() override
	{
		projectileType->update();
	}

	template <typename T, typename... TArgs>
	void setProjectile(TArgs&&... mArgs)
	{
		T* newProjectile(new T(std::forward<TArgs>(mArgs)...));
		newProjectile->projectileComponent = this;

		projectileType = newProjectile;

		newProjectile->init(entity, targets);
	}

	template<typename T> T& getProjectile() const
	{
		return *static_cast<T*>(projectileType);
	}

	std::vector<Entity*>* getTargets()
	{
		return targets;
	}

private:
	ProjectileBase* projectileType = nullptr;

	std::vector<Entity*>* targets;
};