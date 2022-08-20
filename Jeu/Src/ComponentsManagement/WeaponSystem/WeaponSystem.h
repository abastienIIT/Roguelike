#pragma once
#include <map>
#include "../ECS.h"
#include "../Animation/Animation.h"

class WeaponBase;
class WeaponComponent;

class WeaponBase
{
public:
	WeaponComponent* weaponComponent = nullptr;

	WeaponBase() {}
	virtual ~WeaponBase() {}

	void init(Entity* owner, std::vector<Entity*>* targets, int slot)
	{
		this->owner = owner;
		this->targets = targets;
		this->slot = slot;
	}

	virtual void update() {}

	virtual void attackPressed(int attackID) {}
	virtual void attackRealeased(int attackID) {}
	virtual void attackInterrupt() {}

	virtual void drawHitbox() {}

protected:
	Entity* owner = nullptr;
	std::vector<Entity*>* targets = nullptr;
	int slot = false;
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
		if (weapon != nullptr) weapon->update();
		if (weapon2 != nullptr) weapon2->update();
	}
	void draw() override
	{
		if (weapon != nullptr) weapon->drawHitbox();
		if (weapon2 != nullptr) weapon2->drawHitbox();
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

	WeaponBase* getWeapon(bool slot2 = false) const
	{
		if (!slot2) return weapon;
		else return weapon2;
	}

private:
	WeaponBase* weapon = nullptr;
	WeaponBase* weapon2 = nullptr;

	std::vector<Entity*>* targets;
};
