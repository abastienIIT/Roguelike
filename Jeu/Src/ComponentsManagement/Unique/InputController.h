#pragma once

#include <vector>

#include "../../ComponentsManagement/ActionsComponent/ActionsComponent.h"
#include "../../EventHandler/eventHandler.h"

class InputController : public Component
{
public:
	ActionsComponent* actions;

	void init() override;
	void update() override;

	bool pause = false;
	bool pauseMode = false;

	bool displayMap = false;

	bool testButton = false;
private:
	enum flags
	{
		STOP = 0,
		LEFT = -1,
		RIGHT = 1
	};
	
	EventHandler *eventHandler;
	bool previousSpaceState = false;
	bool previousEState = false;

	bool previousWState = false;
};
