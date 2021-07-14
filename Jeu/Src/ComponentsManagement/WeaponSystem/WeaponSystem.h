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
	void setWeapon(TArgs&&... mArgs, bool slot2 = false)
	{
		T* newWeapon(new T(std::forward<TArgs>(mArgs)...));
		newWeapon->weaponComponent = this;

		if (!slot2) weapon = newWeapon;
		else weapon2 = newWeapon;

		newWeapon->init(entity, targets, slot2 + 1);
	}

	template<typename T> T& getWeapon(bool slot2 = false) const
	{
		if (!slot2) return *static_cast<T*>(weapon);
		else return *static_cast<T*>(weapon2);
	}

private:
	WeaponBase* weapon = nullptr;
	WeaponBase* weapon2 = nullptr;

	std::vector<Entity*>* targets;
};
