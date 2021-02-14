#pragma once

#include "../Components.h"
#include "eventHandler.h"
#include <vector>

class InputController : public Component
{
public:
	ActionsComponent* actions;

	void init() override
	{
		actions = &entity->getComponent<ActionsComponent>();
		eventHandler.setRepeatKeyButton(true, SDL_SCANCODE_D);
		eventHandler.setRepeatKeyButton(true, SDL_SCANCODE_A);
		eventHandler.setRepeatJoystickHat(true, 0);
	}

	void update() override;

private:
	enum flags
	{
		STOP = 0,
		LEFT = -1,
		RIGHT = 1
	};
	EventHandler eventHandler;
};
