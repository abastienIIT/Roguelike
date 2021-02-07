#pragma once

#include "Components.h"

class InputController : public Component
{
public:
	ActionsComponent* actions;

	InputController()
	{
	    keyboardState = SDL_GetKeyboardState(NULL);
        myQuit = false;
        for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
        {
            repeatKeyButton[i] = false;
        }
        for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
        {
            myKeyButton[i] = false;
        }
        for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
        {
            myKeyButtonOff[i] = false;
        }
	}

	void init() override
	{
		actions = &entity->getComponent<ActionsComponent>();
		setRepeatKeyButton(true, SDL_SCANCODE_D);
		setRepeatKeyButton(true, SDL_SCANCODE_Q);
	}

	void update() override;

private:
    void updateEvents();
    void setRepeatKeyButton(bool statut);
    void setRepeatKeyButton(bool statut, SDL_Scancode key);
    bool getKeyState(SDL_Scancode key);
    bool quit(void);
	enum flags
	{
		STOP = 0,
		LEFT = -1,
		RIGHT = 1
	};
    SDL_Event myEvent;
    const Uint8* keyboardState;
    bool repeatKeyButton[SDL_NUM_SCANCODES];
    bool myKeyButton[SDL_NUM_SCANCODES];
    bool myKeyButtonOff[SDL_NUM_SCANCODES];
    bool myQuit;
};
