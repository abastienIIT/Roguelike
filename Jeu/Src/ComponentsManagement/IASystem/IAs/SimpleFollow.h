#pragma once

#include "../IAs.h"

class SimpleFollow : public IABase
{
public:
	SimpleFollow() {}

	void update() override;
};
