#pragma once

#include "../Components.h"
#include "joystick.hpp"
#include <vector>

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

	~InputController()
	{
	    for (unsigned int i = 0 ; i < tableauJoystick.size() ; i++)
        {
            if (tableauJoystick[i] != NULL)
            {
                delete(tableauJoystick[i]);
            }
        }
	}

	void init() override
	{
		actions = &entity->getComponent<ActionsComponent>();
		setRepeatKeyButton(true, SDL_SCANCODE_D);
		setRepeatKeyButton(true, SDL_SCANCODE_A);
		setRepeatJoystickButton(true, 2);
		setRepeatJoystickButton(true, 3);
		loadJoysticks();
        for (int i = 0 ; i < 30 ; i++)
        {
            repeatJoyButton[i] = false;
        }

        for (unsigned int i = 0 ; i < tableauJoystick.size() ; i++)
        {
            myJoyButtonOff.push_back(std::vector<bool>(false));
            for (int j = 0 ; j < tableauJoystick[i]->getNbButton() ; j++)
            {
                myJoyButtonOff[i].push_back(false);
            }
        }

        for (unsigned int i = 0 ; i < tableauJoystick.size() ; i++)
        {
            myJoyButton.push_back(std::vector<bool>(false));
            for (int j = 0 ; j < tableauJoystick[i]->getNbButton() ; j++)
            {
                myJoyButton[i].push_back(false);
            }
        }

        for (unsigned int i = 0 ; i < tableauJoystick.size() ; i++)
        {
            myJoyAxis.push_back(std::vector<int>(0));
            for (int j = 0 ; j < tableauJoystick[i]->getNbAxis() ; j++)
            {
                myJoyAxis[i].push_back(0);
            }
        }
	}

	void update() override;

private:
    void updateEvents();
    void setRepeatKeyButton(bool statut);
    void setRepeatKeyButton(bool statut, SDL_Scancode key);
    bool getKeyState(SDL_Scancode key);
    void loadJoysticks(void);
    void setRepeatJoystickButton(bool statut);
    void setRepeatJoystickButton(bool statut, unsigned int button);
    bool getJoyButtonState(int joystick, unsigned int button);
    int getJoyAxisState(int joystick, unsigned int axis);
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
    std::vector<Joystick*> tableauJoystick;
    bool repeatJoyButton[30];
    std::vector< std::vector<bool> > myJoyButtonOff;
    std::vector< std::vector<bool> > myJoyButton;
    std::vector< std::vector<int> > myJoyAxis;
};
