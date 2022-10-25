#pragma once

#include "../TrapSystem.h"

class SpikeTrap : public TrapBase
{
public:
	SpikeTrap() {}

	void init(Entity* owner, std::vector<Entity*> targets);
	void update() override;

	void growTrap(Entity* newTrapSection, int widthToAdd);

	bool checkTargetOnTrap();
	void playAnimForAllSections(std::string animName);

private:
	std::vector<Entity*> fullTrap;

	enum states {Waiting, Triggering, Triggered, Reseting};
	int trapState = states::Waiting;
	int triggerTime;
};

