#pragma once

#include "../IAs.h"

class SimpleFollow : public IAs
{
public:
	SimpleFollow() {}

	void init(Entity* owner, Entity* target)
	{
		IAs::init(owner, target);
	}

	void update() override;
};
