#pragma once
#include <map>
#include "../ECS.h"
#include "../Animation/Animation.h"

class WeaponBase;
class WeaponComponent;

class WeaponBase
{
public:
	WeaponComponent* weaponComponent;

	WeaponBase() {}
	virtual ~WeaponBase() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot)
	{
		this->owner = owner;
		this->targets = targets;
		this->slot = slot;
	}

	virtual void update() {}

	virtual void attackPressed() {}
	virtual void attackRealeased() {}
	virtual void attackSpecialPressed() {}
	virtual void attackSpecialRealeased() {}

protected:
	Entity* owner = nullptr;
	std::vector<Entity*>* targets;
	int slot;
	std::map<std::string, Animation> animations;
};

class WeaponComponent : public Component
{
public:
	WeaponComponent(Entity* target) {targets->emplace_back(target); }
	WeaponComponent(std::vector<Entity*>* targets) : targets(targets) {}
	~WeaponComponent() {}

	void init() override {}
	void update() override
	{
		weapon->update();
		weapon2->update();
	}

	template <typename T, typename... TArgs>
	void setWeapon(TArgs&&... mArgs)
	{
		T* newWeapon(new T(std::forward<TArgs>(mArgs)...));
		newWeapon->weaponComponent = this;
		weapon = newWeapon;

		newWeapon->init(entity, targets, 1);
	}

	template<typename T> T& getWeapon() const
	{
		return *static_cast<T*>(weapon);
	}

	template <typename T, typename... TArgs>
	void setWeapon2(TArgs&&... mArgs)
	{
		T* newWeapon(new T(std::forward<TArgs>(mArgs)...));
		newWeapon->weaponComponent = this;
		weapon2 = newWeapon;

		newWeapon->init(entity, targets, 2);
	}

	template<typename T> T& getWeapon2() const
	{
		return *static_cast<T*>(weapon2);
	}


private:
	WeaponBase* weapon = nullptr;
	WeaponBase* weapon2 = nullptr;

	std::vector<Entity*>* targets;
};
