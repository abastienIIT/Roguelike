#pragma once

#include "../IASystem.h"

class BigKnightIA : public IABase
{
public:
	BigKnightIA() {}

	void init(Entity* owner, std::vector<Entity*> targets);
	void update() override;

private:
	int lastTime = 0;
	bool attackDone = false;
};

