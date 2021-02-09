#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "joystickHandler.h"

class EventHandler
{
    public:
        EventHandler();
        ~EventHandler();

        void updateEvents(void); // Méthode qui met la liste d'évènements à jour.
        bool quit(void); // Méthode qui indique si on quitte la boucle (evenement de la fenetre). True = quitter.
        void setRepeatKeyButton(bool statut); // Méthode qui défini si les touches du clavier sont répétées ou non.
        void setRepeatKeyButton(bool statut, SDL_Scancode key); // Méthode qui défini si la touche du clavier est répétée ou non.
        void setRepeatMouseButton(bool statut); // Méthode qui défini si les boutons de la souris sont répétés ou non.
        void setRepeatMouseButton(bool statut, Uint8 button); // Méthode qui défini si le bouton de la souris est répété ou non.
        bool getKeyState(SDL_Scancode key); // Méthode qui renvoie le statut d'une touche du clavier. True = touche enfoncée.
        bool getMouseButtonState(Uint8 button); // Méthode qui renvoie le statut d'un bouton de la souris. True = bouton enfoncé.
        bool mouseMoving(void); // Méthode qui indique si la position de la souris a changée. True = position changée.
        int getMouseX(void); // Méthode qui permet d'obtenir la position X de la souris.
        int getMouseY(void); // Méthode qui permet d'obtenir la position Y de la souris.
        int getMouseRelX(void); // Méthode qui permet d'obtenir la position X Relative de la souris.
        int getMouseRelY(void); // Méthode qui permet d'obtenir la position Y relative de la souris.
        void showCursor(bool state); // Méthode qui permet de montrer ou cacher le curseur de la souris.
        void captureCursor(bool state); // Permet d'activer le mode relatif de la souris.
        void setRepeatJoystickButton(bool statut); // Méthode qui défini si les boutons du joystick sont répétées ou non.
        void setRepeatJoystickButton(bool statut, unsigned int button); // Méthode qui défini si le bouton du joystick indiqué est répété ou non.
        void setRepeatJoystickHat(bool statut); // Méthode qui défini si les Hats du joystick sont répétés ou non.
        void setRepeatJoystickHat(bool statut, unsigned int hat); // Méthode qui défini si le Hat du joystick indiqué est répété ou non.
        bool getJoyButtonState(int joystick, unsigned int button); // Méthode qui renvoie le statut d'un bouton du joystick. True = touche enfoncée.
        int getJoyAxisState(int joystick, unsigned int axis); // Méthode qui renvoie la valeur d'un axe du joystick.
        int getJoyHatsState(int joystick, unsigned int hats); // Méthode qui renvoie la valeur d'une croix multidirectionnelle du joystick (sans répétition).
    protected:
    private:
        SDL_Event myEvent;
        JoystickHandler *myJoystickHandler;
        const Uint8* keyboardState;
        Uint32 mouseState;
        bool repeatKeyButton[SDL_NUM_SCANCODES];
        bool myKeyButton[SDL_NUM_SCANCODES];
        bool myKeyButtonOff[SDL_NUM_SCANCODES];
        bool repeatMouseButton[8];
        bool myMouseButton[8];
        bool myMouseButtonOff[8];
        int myMouseX;
        int myMouseY;
        int myMouseRelX;
        int myMouseRelY;
        bool myQuit;
        bool repeatJoyButton[30];
        bool repeatJoyHat[30];
        std::vector< std::vector<bool> > myJoyButtonOff;
        std::vector< std::vector<bool> > myJoyButton;
        std::vector< std::vector<int> > myJoyAxis;
        std::vector< std::vector<int> > myJoyNewHat;
        std::vector< std::vector<int> > myJoyOldHat;
        std::vector< std::vector<int> > myJoyHat;
};

