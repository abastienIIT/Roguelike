#include "joystick.hpp"

Joystick::Joystick()
{
    joystick = NULL;
}

Joystick::~Joystick()
{
    if (SDL_JoystickGetAttached(joystick))
    {
        SDL_JoystickClose(joystick);
    }
}

bool Joystick::setJoystick(int deviceIndex)
{
    joystick = SDL_JoystickOpen(deviceIndex);
    if (joystick == NULL)
    {
        return false;
    }
    nbButton = SDL_JoystickNumButtons(joystick);
    if (nbButton < 0)
    {
        return false;
    }
    nbAxis = SDL_JoystickNumAxes(joystick);
    if (nbAxis < 0)
    {
        return false;
    }
    return true;
}

SDL_Joystick *Joystick::getJoystick(void)
{
    if (SDL_JoystickGetAttached(joystick))
    {
        return joystick;
    }
    return NULL;
}

int Joystick::getNbButton(void)
{
    return nbButton;
}

int Joystick::getNbAxis(void)
{
    return nbAxis;
}

int Joystick::getNbHat(void)
{
    return nbHat;
}
