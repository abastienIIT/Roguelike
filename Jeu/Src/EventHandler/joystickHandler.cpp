#include "joystickHandler.h"

JoystickHandler::JoystickHandler()
{

}

JoystickHandler::~JoystickHandler()
{
    for (unsigned int i = 0 ; i < tableauJoystick.size() ; i++)
    {
        if (tableauJoystick[i] != NULL)
        {
            delete(tableauJoystick[i]);
        }
    }
}

bool JoystickHandler::loadJoysticks(void)
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
            return false;
        }
    }
    return true;
}

int JoystickHandler::getNumberJoystick(void)
{
    return SDL_NumJoysticks();
}

SDL_Joystick *JoystickHandler::getJoystick(int deviceIndex)
{
    if (deviceIndex < SDL_NumJoysticks())
    {
        return tableauJoystick[deviceIndex]->getJoystick();
    }
    return NULL;
}

int JoystickHandler::getNbButton(int deviceIndex)
{
    if (deviceIndex < SDL_NumJoysticks())
    {
        return tableauJoystick[deviceIndex]->getNbButton();
    }
    return 0;
}

int JoystickHandler::getNbAxis(int deviceIndex)
{
    if (deviceIndex < SDL_NumJoysticks())
    {
        return tableauJoystick[deviceIndex]->getNbAxis();
    }
    return 0;
}

int JoystickHandler::getNbHat(int deviceIndex)
{
    if (deviceIndex < SDL_NumJoysticks())
    {
        return tableauJoystick[deviceIndex]->getNbHat();
    }
    return 0;
}
