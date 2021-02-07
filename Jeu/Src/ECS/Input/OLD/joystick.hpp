#pragma once

#include <SDL2/SDL.h>

class Joystick
{
    public:
        /* Méthodes */
        Joystick();
        ~Joystick();
        bool setJoystick(int deviceIndex);
        SDL_Joystick *getJoystick(void);
        int getNbButton(void);
        int getNbAxis(void);
        int getNbHat(void);
    protected:
    private:
        /* Variables */
        SDL_Joystick *joystick;
        int nbButton;
        int nbAxis;
        int nbHat;
};
