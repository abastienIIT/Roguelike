#include "eventHandler.h"

EventHandler::EventHandler()
{
    int i;

    lastDevice = -1;
    lastDeviceRemoved = -1;
    myJoystickHandler = new JoystickHandler();
    keyboardState = SDL_GetKeyboardState(NULL);
    mouseState = SDL_GetMouseState(&myMouseX, &myMouseY);
    myMouseX = 0;
    myMouseY = 0;
    myMouseRelX = 0;
    myMouseRelY = 0;
    for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
    {
        repeatKeyButton[i] = false;
    }
    myQuit = false;
    for (i = 0 ; i < SDL_NUM_SCANCODES ; i++)
    {
        myKeyButton[i] = false;
    }
    for (i = 0 ; i < SDL_NUM_SCANCODES ; i++)
    {
        myKeyButtonOff[i] = false;
    }
    for (int i = 0 ; i < 8 ; i++)
    {
        repeatMouseButton[i] = false;
    }
    for (i = 0 ; i < 8 ; i++)
    {
        myMouseButton[i] = false;
    }
    for (i = 0 ; i < 8 ; i++)
    {
        myMouseButtonOff[i] = false;
    }
    for (int i = 0 ; i < 30 ; i++)
    {

        repeatJoyButton[i] = false;
    }

    myJoystickHandler->loadJoysticks();
    for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
    {
        myJoyButtonOff.push_back(std::vector<bool>(false));
        for (int j = 0 ; j < myJoystickHandler->getNbButton(i) ; j++)
        {
            myJoyButtonOff[i].push_back(false);
        }
    }

    for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
    {
        myJoyButton.push_back(std::vector<bool>(false));
        for (int j = 0 ; j < myJoystickHandler->getNbButton(i) ; j++)
        {
            myJoyButton[i].push_back(false);
        }
    }

    for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
    {
        myJoyAxis.push_back(std::vector<int>(0));
        for (int j = 0 ; j < myJoystickHandler->getNbAxis(i) ; j++)
        {
            myJoyAxis[i].push_back(0);
        }
    }

    for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
    {
        myJoyHat.push_back(std::vector<int>(0));
        for (int j = 0 ; j < myJoystickHandler->getNbHat(i) ; j++)
        {
            myJoyHat[i].push_back(0);
        }
    }

    for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
    {
        myJoyNewHat.push_back(std::vector<int>(0));
        for (int j = 0 ; j < myJoystickHandler->getNbHat(i) ; j++)
        {
            myJoyNewHat[i].push_back(0);
        }
    }

    for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
    {
        myJoyOldHat.push_back(std::vector<int>(0));
        for (int j = 0 ; j < myJoystickHandler->getNbHat(i) ; j++)
        {
            myJoyOldHat[i].push_back(0);
        }
    }
}

EventHandler::~EventHandler()
{
    delete(myJoystickHandler);
}

void EventHandler::updateEvents(void)
{
    int i;
    myMouseRelX = 0;
    myMouseRelY = 0;

    SDL_PollEvent(&myEvent);
    /// Clavier
    if (repeatKeyButton)
    {
        for (i = 0 ; i < SDL_NUM_SCANCODES ; i++)
        {
            if (keyboardState[i])
            {
                myKeyButton[i] = true;
            }
            else
            {
                myKeyButton[i] = false;
            }
        }
    }
    else
    {
        for (i = 0 ; i < SDL_NUM_SCANCODES ; i++)
        {
            if (keyboardState[i])
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
            else
            {
                myKeyButton[i] = false;
                myKeyButtonOff[i] = false;
            }
        }
    }


    /// Souris
    mouseState = SDL_GetMouseState(&myMouseX, &myMouseY);
    SDL_GetRelativeMouseState(&myMouseRelX,&myMouseRelY);
    if (repeatMouseButton)
    {
        // Clic gauche
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            myMouseButton[SDL_BUTTON_LEFT] = true;
        }
        else
        {
            myMouseButton[SDL_BUTTON_LEFT] = false;
        }
        // Clic droit
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            myMouseButton[SDL_BUTTON_RIGHT] = true;
        }
        else
        {
            myMouseButton[SDL_BUTTON_RIGHT] = false;
        }
        // Clic molette
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        {
            myMouseButton[SDL_BUTTON_MIDDLE] = true;
        }
        else
        {
            myMouseButton[SDL_BUTTON_MIDDLE] = false;
        }
    }
    else
    {
        // Clic gauche
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if (!myMouseButtonOff[SDL_BUTTON_LEFT])
            {
                myMouseButton[SDL_BUTTON_LEFT] = true;
                myMouseButtonOff[SDL_BUTTON_LEFT] = true;
            }
            else
            {
                myMouseButton[SDL_BUTTON_LEFT] = false;
            }
        }
        else
        {
            myMouseButton[SDL_BUTTON_LEFT] = false;
            myMouseButtonOff[SDL_BUTTON_LEFT] = false;
        }
        // Clic droit
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
        {
            if (!myMouseButtonOff[SDL_BUTTON_RIGHT])
            {
                myMouseButton[SDL_BUTTON_RIGHT] = true;
                myMouseButtonOff[SDL_BUTTON_RIGHT] = true;
            }
            else
            {
                myMouseButton[SDL_BUTTON_RIGHT] = false;
            }
        }
        else
        {
            myMouseButton[SDL_BUTTON_RIGHT] = false;
            myMouseButtonOff[SDL_BUTTON_RIGHT] = false;
        }
        // Clic molette
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE))
        {
            if (!myMouseButtonOff[SDL_BUTTON_MIDDLE])
            {
                myMouseButton[SDL_BUTTON_MIDDLE] = true;
                myMouseButtonOff[SDL_BUTTON_MIDDLE] = true;
            }
            else
            {
                myMouseButton[SDL_BUTTON_MIDDLE] = false;
            }
        }
        else
        {
            myMouseButton[SDL_BUTTON_MIDDLE] = false;
            myMouseButtonOff[SDL_BUTTON_MIDDLE] = false;
        }
    }
    /// Joystick
    // Branchement ou débranchement d'un joystick
    if ((myEvent.jdevice.type == SDL_JOYDEVICEADDED && myEvent.jdevice.which != lastDevice) || (myEvent.jdevice.type == SDL_JOYDEVICEREMOVED && myEvent.jdevice.which != lastDeviceRemoved))
    {
        if(myEvent.jdevice.type == SDL_JOYDEVICEADDED)
        {
            lastDevice = myEvent.jdevice.which;
            lastDeviceRemoved = -1;
        }

        if(myEvent.jdevice.type == SDL_JOYDEVICEREMOVED)
        {
            lastDevice = -1;
            lastDeviceRemoved = myEvent.jdevice.which;
        }


        myJoystickHandler->loadJoysticks();

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

        for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
        {
            myJoyButtonOff.push_back(std::vector<bool>(false));
            for (int j = 0 ; j < myJoystickHandler->getNbButton(i) ; j++)
            {
                myJoyButtonOff[i].push_back(false);
            }
        }

        for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
        {
            myJoyButton.push_back(std::vector<bool>(false));
            for (int j = 0 ; j < myJoystickHandler->getNbButton(i) ; j++)
            {
                myJoyButton[i].push_back(true);
            }
        }

        for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
        {
            myJoyAxis.push_back(std::vector<int>(0));
            for (int j = 0 ; j < myJoystickHandler->getNbAxis(i) ; j++)
            {
                myJoyAxis[i].push_back(0);
            }
        }

        for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
        {
            myJoyHat.push_back(std::vector<int>(0));
            for (int j = 0 ; j < myJoystickHandler->getNbHat(i) ; j++)
            {
                myJoyHat[i].push_back(0);
            }
        }

        for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
        {
            myJoyNewHat.push_back(std::vector<int>(0));
            for (int j = 0 ; j < myJoystickHandler->getNbHat(i) ; j++)
            {
                myJoyNewHat[i].push_back(0);
            }
        }

        for (int i = 0 ; i < myJoystickHandler->getNumberJoystick() ; i++)
        {
            myJoyOldHat.push_back(std::vector<int>(0));
            for (int j = 0 ; j < myJoystickHandler->getNbHat(i) ; j++)
            {
                myJoyOldHat[i].push_back(0);
            }
        }
    }

    // Boutons
    for (unsigned i = 0 ; i < myJoyButton.size() ; i++)
    {
        for (unsigned  j = 0 ; j < myJoyButton[i].size() ; j++)
        {
            if (SDL_JoystickGetButton(myJoystickHandler->getJoystick(i), j))
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
    for (unsigned i = 0 ; i < myJoyAxis.size() ; i++)
    {
        for (unsigned  j = 0 ; j < myJoyAxis[i].size() ; j++)
        {
            myJoyAxis[i][j] = SDL_JoystickGetAxis(myJoystickHandler->getJoystick(i), j);
        }
    }

    // Hats
    for (unsigned i = 0 ; i < myJoyHat.size() ; i++)
    {
        for (unsigned  j = 0 ; j < myJoyHat[i].size() ; j++)
        {
            myJoyNewHat[i][j] = SDL_JoystickGetHat(myJoystickHandler->getJoystick(i), j);
            if (repeatJoyHat[i] == false)
            {
                if(myJoyNewHat[i][j] == myJoyOldHat[i][j])
                {
                    myJoyHat[i][j] = SDL_HAT_CENTERED;
                }
                else
                {
                    myJoyHat[i][j] = myJoyNewHat[i][j];
                    myJoyOldHat[i][j] = myJoyNewHat[i][j];
                }
            }
            else
            {
                myJoyHat[i][j] = myJoyNewHat[i][j];
            }
        }
    }

    /// Fenêtre
    if (myEvent.window.event == SDL_WINDOWEVENT_CLOSE)
    {
        myQuit = true;
    }
}

bool EventHandler::quit(void)
{
    return myQuit;
}

void EventHandler::setRepeatKeyButton(bool statut)
{
    for (int i = 0 ; i < SDL_NUM_SCANCODES ; i++)
    {
        repeatKeyButton[i] = statut;
    }
}

void EventHandler::setRepeatKeyButton(bool statut, SDL_Scancode key)
{
    repeatKeyButton[key] = statut;
}

void EventHandler::setRepeatMouseButton(bool statut)
{
    for (int i = 0 ; i < 8 ; i++)
    {
        repeatMouseButton[i] = statut;
    }
}

void EventHandler::setRepeatMouseButton(bool statut, Uint8 button)
{
    repeatMouseButton[button] = statut;
}

bool EventHandler::getKeyState(SDL_Scancode key)
{
    return myKeyButton[key];
}

bool EventHandler::getMouseButtonState(Uint8 button)
{
    return myMouseButton[button];
}

bool EventHandler::mouseMoving(void)
{
    if (myMouseRelX == 0 && myMouseRelY == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int EventHandler::getMouseX(void)
{
    return myMouseX;
}

int EventHandler::getMouseY(void)
{
    return myMouseY;
}

int EventHandler::getMouseRelX(void)
{
    return myMouseRelX;
}

int EventHandler::getMouseRelY(void)
{
    return myMouseRelY;
}

void EventHandler::showCursor(bool state)
{
    if (state)
    {
        SDL_ShowCursor(SDL_ENABLE);
    }
    else
    {
        SDL_ShowCursor(SDL_DISABLE);
    }
}

void EventHandler::captureCursor(bool state)
{
    if (state)
    {
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }
    else
    {
        SDL_SetRelativeMouseMode(SDL_FALSE);
    }
}

void EventHandler::setRepeatJoystickButton(bool statut)
{
    for (unsigned i = 0 ; i < 30 ; i++)
    {
        repeatJoyButton[i] = statut;
    }
}

void EventHandler::setRepeatJoystickButton(bool statut, unsigned int button)
{
    repeatJoyButton[button] = statut;
}

void EventHandler::setRepeatJoystickHat(bool statut)
{
    for (unsigned i = 0 ; i < 30 ; i++)
    {
        repeatJoyHat[i] = statut;
    }
}

void EventHandler::setRepeatJoystickHat(bool statut, unsigned int hat)
{
    repeatJoyHat[hat] = statut;
}

bool EventHandler::getJoyButtonState(int joystick, unsigned int button)
{
    if (joystick < myJoystickHandler->getNumberJoystick())
    {
        if (button < myJoyButton[joystick].size())
        {
            return myJoyButton[joystick][button];
        }
        return false;
    }
    return false;
}

int EventHandler::getJoyAxisState(int joystick, unsigned int axis)
{
    if (joystick < myJoystickHandler->getNumberJoystick())
    {
        if (axis < myJoyAxis[joystick].size())
        {
            return myJoyAxis[joystick][axis];
        }
        return 0;
    }
    return 0;
}

int EventHandler::getJoyHatsState(int joystick, unsigned int hat)
{
    if (joystick < myJoystickHandler->getNumberJoystick())
    {
        if (hat < myJoyHat[joystick].size())
        {
            return myJoyHat[joystick][hat];
        }
        return 0;
    }
    return 0;
}
