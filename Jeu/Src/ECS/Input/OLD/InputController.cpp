#include "../../Game.h"
#include "InputController.h"

void InputController::update()
{
    updateEvents();

    if (quit() == true) Game::isRunning = false;
    if (getKeyState(SDL_SCANCODE_ESCAPE) == true) Game::isRunning = false;

    if (getKeyState(SDL_SCANCODE_W) == true || getJoyButtonState(0, 0) == true) actions->jumpStart();
    if (getKeyState(SDL_SCANCODE_A) == true || getJoy(0, 2) == true) actions->walk(LEFT);
    if (getKeyState(SDL_SCANCODE_D) == true || getJoyButtonState(0, 3) == true) actions->walk(RIGHT);
    if (getKeyState(SDL_SCANCODE_SPACE) == true || getJoyButtonState(0, 1) == true) actions->swordAttack();
}

void InputController::updateEvents()
{
    SDL_PollEvent(&myEvent);

    /// Clavier
    for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
    {
        if (keyboardState[i])
        {
            if (repeatKeyButton[i])
            {
                myKeyButton[i] = true;
            }
            else
            {
                if (!myKeyButtonOff[i])
                {
                    myKeyButton[i] = true;
                    myKeyButtonOff[i] = true;
                }
                else
                {
                    myKeyButton[i] = false;
                }
            }
        }
        else
        {
            if (repeatKeyButton[i])
            {
                myKeyButton[i] = false;
            }
            else
            {
                myKeyButton[i] = false;
                myKeyButtonOff[i] = false;
            }
        }
    }
    /// Joystick
    // Branchement ou débranchement d'un joystick
    if (myEvent.jdevice.type == SDL_JOYDEVICEADDED || myEvent.jdevice.type == SDL_JOYDEVICEREMOVED)
    {
        loadJoysticks();

        for (unsigned int i = 0 ; i < myJoyButtonOff.size() ; i++)
        {
            myJoyButtonOff[i].resize(0);
        }
        myJoyButtonOff.resize(0);

        for (unsigned int i = 0 ; i < myJoyButton.size() ; i++)
        {
            myJoyButton[i].resize(0);
        }
        myJoyButton.resize(0);

        for (unsigned int i = 0 ; i < myJoyAxis.size() ; i++)
        {
            myJoyAxis[i].resize(0);
        }
        myJoyAxis.resize(0);

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

    // Boutons
    for (unsigned int i = 0 ; i < myJoyButton.size() ; i++)
    {
        for (unsigned int j = 0 ; j < myJoyButton[i].size() ; j++)
        {
            if (SDL_JoystickGetButton(tableauJoystick[i]->getJoystick(), j))
            {
                if (repeatJoyButton[j])
                {
                    myJoyButton[i][j] = true;
                }
                else
                {
                    if (!myJoyButtonOff[i][j])
                    {
                        myJoyButton[i][j] = true;
                        myJoyButtonOff[i][j] = true;
                    }
                    else
                    {
                        myJoyButton[i][j] = false;
                    }
                }
            }
            else
            {
                if (repeatJoyButton[j])
                {
                    myJoyButton[i][j] = false;
                }
                else
                {
                    myJoyButton[i][j] = false;
                    myJoyButtonOff[i][j] = false;
                }
            }
        }
    }

    // Axes
    for (unsigned int i = 0 ; i < myJoyAxis.size() ; i++)
    {
        for (unsigned int j = 0 ; j < myJoyAxis[i].size() ; j++)
        {
            myJoyAxis[i][j] = SDL_JoystickGetAxis(tableauJoystick[i]->getJoystick(), j);
        }
    }

    /// Fenêtre
    if (myEvent.type == SDL_QUIT)
    {
        myQuit = true;
    }
}

void InputController::setRepeatKeyButton(bool statut)
{
    for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
    {
        repeatKeyButton[i] = statut;
    }

}

void InputController::setRepeatKeyButton(bool statut, SDL_Scancode key)
{
    repeatKeyButton[key] = statut;
}

bool InputController::getKeyState(SDL_Scancode key)
{
    return myKeyButton[key];
}

void InputController::loadJoysticks(void)
{
    if (tableauJoystick.size() > 0)
    {
        for (int i = tableauJoystick.size() - 1 ; i >= 0 ; i--)
        {
            if (tableauJoystick[i] != NULL)
            {
                delete(tableauJoystick[i]);
                tableauJoystick.pop_back();
            }
        }
    }
    for (int i = 0 ; i < SDL_NumJoysticks() ; i++)
    {
        tableauJoystick.push_back(new Joystick());
        if (!tableauJoystick[i]->setJoystick(i))
        {
            tableauJoystick.pop_back();
        }
    }
}

void InputController::setRepeatJoystickButton(bool statut)
{
    for (unsigned i = 0 ; i < 30 ; i++)
    {
        repeatJoyButton[i] = statut;
    }
}

void InputController::setRepeatJoystickButton(bool statut, unsigned int button)
{
    repeatJoyButton[button] = statut;
}

bool InputController::getJoyButtonState(int joystick, unsigned int button)
{
    if (joystick < tableauJoystick.size())
    {
        if (button < myJoyButton[joystick].size())
        {
            return myJoyButton[joystick][button];
        }
        return false;
    }
    return false;
}

int InputController::getJoyAxisState(int joystick, unsigned int axis)
{
    if (joystick < tableauJoystick.size())
    {
        if (axis < myJoyAxis[joystick].size())
        {
            return myJoyAxis[joystick][axis];
        }
        return 0;
    }
    return 0;
}

bool InputController::quit(void)
{
    return myQuit;
}
