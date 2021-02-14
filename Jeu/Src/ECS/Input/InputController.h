#pragma once

#include "../Components.h"
#include "../../Globalbilboulga.h"
#include <vector>

class InputController : public Component
{
public:
	ActionsComponent* actions;

	void init() override
	{
        globalbilboulga = Globalbilboulga::getInstance();

		actions = &entity->getComponent<ActionsComponent>();
		globalbilboulga->getEventHandler()->setRepeatKeyButton(true, SDL_SCANCODE_D);
		globalbilboulga->getEventHandler()->setRepeatKeyButton(true, SDL_SCANCODE_A);
		globalbilboulga->getEventHandler()->setRepeatJoystickHat(true, 0);
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
};
