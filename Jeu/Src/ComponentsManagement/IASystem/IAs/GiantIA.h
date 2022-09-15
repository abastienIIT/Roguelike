#pragma once

#include "../IASystem.h"

class GiantIA : public IABase
{
public:
	GiantIA() {}

	void init(Entity* owner, std::vector<Entity*> targets);
	void update() override;

private:
	int lastTime = 0;
	bool attackDone = false;
};

