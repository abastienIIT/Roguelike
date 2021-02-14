#pragma once

#include <SDL2/SDL.h>

class Joystick
{
    public:
        Joystick();
        ~Joystick();
        bool setJoystick(int deviceIndex); // Ouverture du joystick. Renvoie false s'il y a eu une erreur et true si la méthode s'est déroulée correctement.
        SDL_Joystick *getJoystick(void); // Récupération du joystick. Renvoie NULL si le Joystick n'existe pas.
        int getNbButton(void); // Récupération du nombre de boutons du Joystick.
        int getNbAxis(void); // Récupération du nombre d'axes du Joystick.
        int getNbHat(void); // Récupération du nombre de croix multidirectionnelles du Joystick.
    private:
        SDL_Joystick *joystick;
        int nbButton;
        int nbAxis;
        int nbHat;
};

