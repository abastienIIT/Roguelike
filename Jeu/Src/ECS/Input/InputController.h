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
        Globalbilboulga = Globalbilboulga::getInstance();

		actions = &entity->getComponent<ActionsComponent>();
		Globalbilboulga->getEventHandler()->setRepeatKeyButton(true, SDL_SCANCODE_D);
		Globalbilboulga->getEventHandler()->setRepeatKeyButton(true, SDL_SCANCODE_A);
		Globalbilboulga->getEventHandler()->setRepeatJoystickHat(true, 0);
	}

	void update() override;

private:
	enum flags
	{
		STOP = 0,
		LEFT = -1,
		RIGHT = 1
	};
	Globalbilboulga *Globalbilboulga;
};
