#pragma once

#include "Components.h"

class KeyboardController : public Component
{
public:
	ActionsComponent* actions;

	void init() override
	{
		actions = &entity->getComponent<ActionsComponent>();
	}

	void update() override;

private:
	enum flags
	{
		STOP = 0,
		LEFT = -1,
		RIGHT = 1
	};
};
