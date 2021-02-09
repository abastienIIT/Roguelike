#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "joystick.h"

class JoystickHandler
{
    public:
        JoystickHandler();
        ~JoystickHandler();

        bool loadJoysticks(void); // Charge tous les joysticks. S'ils ont déjà été chargé, les recharge.
        int getNumberJoystick(void); // Permet d'obtenir le nombre de Joystick branchés.
        SDL_Joystick *getJoystick(int deviceIndex); // Permet d'obtenir un pointeur sur le Joystick de l'index indiqué.
        int getNbButton(int deviceIndex); // Récupération du nombre de boutons du Joystick de l'index indiqué.
        int getNbAxis(int deviceIndex); // Récupération du nombre d'axes du Joystick de l'index indiqué.
        int getNbHat(int deviceIndex); // Récupération du nombre de croix multidirectionnelles du Joystick de l'index indiqué.
    protected:
    private:
        std::vector<Joystick*> tableauJoystick;
};
