#pragma once

#include "../Components.h"
#include "../../Common/Globalbilboulga.h"
#include <vector>

class InputController : public Component
{
public:
	ActionsComponent* actions;

	void init() override
	{
        globalbilboulga = Globalbilboulga::getInstance();
        eventHandler = globalbilboulga->getEventHandler();

		actions = &entity->getComponent<ActionsComponent>();
		eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_D);
		eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_A);
		eventHandler->setRepeatJoystickHat(true, 0);
	}

	void update() override;

private:
	enum flags
	{
		STOP = 0,
		LEFT = -1,
		RIGHT = 1
	};
	Globalbilboulga *globalbilboulga;
	EventHandler *eventHandler;
};
