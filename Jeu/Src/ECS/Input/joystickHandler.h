#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "joystick.h"

class JoystickHandler
{
    public:
        JoystickHandler();
        ~JoystickHandler();

        bool loadJoysticks(void); // Charge tous les joysticks. S'ils ont d�j� �t� charg�, les recharge.
        int getNumberJoystick(void); // Permet d'obtenir le nombre de Joystick branch�s.
        SDL_Joystick *getJoystick(int deviceIndex); // Permet d'obtenir un pointeur sur le Joystick de l'index indiqu�.
        int getNbButton(int deviceIndex); // R�cup�ration du nombre de boutons du Joystick de l'index indiqu�.
        int getNbAxis(int deviceIndex); // R�cup�ration du nombre d'axes du Joystick de l'index indiqu�.
        int getNbHat(int deviceIndex); // R�cup�ration du nombre de croix multidirectionnelles du Joystick de l'index indiqu�.
    protected:
    private:
        std::vector<Joystick*> tableauJoystick;
};
