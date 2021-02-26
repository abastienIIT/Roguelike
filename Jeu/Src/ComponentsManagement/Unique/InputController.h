#pragma once

#include <vector>

#include "../../ComponentsManagement/ActionsComponent/ActionsComponent.h"

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
		eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_W);
		eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_A);
		eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_SPACE);
		eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_E);
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
	bool previousSpaceState = false;
	bool previousEState = false;

	bool previousWState = false;
};
