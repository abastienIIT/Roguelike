#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include "joystickHandler.h"

class EventHandler
{
    public:
        EventHandler();
        ~EventHandler();

        void updateEvents(void); // M�thode qui met la liste d'�v�nements � jour.
        bool quit(void); // M�thode qui indique si on quitte la boucle (evenement de la fenetre). True = quitter.
        void setRepeatKeyButton(bool statut); // M�thode qui d�fini si les touches du clavier sont r�p�t�es ou non.
        void setRepeatKeyButton(bool statut, SDL_Scancode key); // M�thode qui d�fini si la touche du clavier est r�p�t�e ou non.
        void setRepeatMouseButton(bool statut); // M�thode qui d�fini si les boutons de la souris sont r�p�t�s ou non.
        void setRepeatMouseButton(bool statut, Uint8 button); // M�thode qui d�fini si le bouton de la souris est r�p�t� ou non.
        bool getKeyState(SDL_Scancode key); // M�thode qui renvoie le statut d'une touche du clavier. True = touche enfonc�e.
        bool getMouseButtonState(Uint8 button); // M�thode qui renvoie le statut d'un bouton de la souris. True = bouton enfonc�.
        bool mouseMoving(void); // M�thode qui indique si la position de la souris a chang�e. True = position chang�e.
        int getMouseX(void); // M�thode qui permet d'obtenir la position X de la souris.
        int getMouseY(void); // M�thode qui permet d'obtenir la position Y de la souris.
        int getMouseRelX(void); // M�thode qui permet d'obtenir la position X Relative de la souris.
        int getMouseRelY(void); // M�thode qui permet d'obtenir la position Y relative de la souris.
        void showCursor(bool state); // M�thode qui permet de montrer ou cacher le curseur de la souris.
        void captureCursor(bool state); // Permet d'activer le mode relatif de la souris.
        void setRepeatJoystickButton(bool statut); // M�thode qui d�fini si les boutons du joystick sont r�p�t�es ou non.
        void setRepeatJoystickButton(bool statut, unsigned int button); // M�thode qui d�fini si le bouton du joystick indiqu� est r�p�t� ou non.
        void setRepeatJoystickHat(bool statut); // M�thode qui d�fini si les Hats du joystick sont r�p�t�s ou non.
        void setRepeatJoystickHat(bool statut, unsigned int hat); // M�thode qui d�fini si le Hat du joystick indiqu� est r�p�t� ou non.
        bool getJoyButtonState(int joystick, unsigned int button); // M�thode qui renvoie le statut d'un bouton du joystick. True = touche enfonc�e.
        int getJoyAxisState(int joystick, unsigned int axis); // M�thode qui renvoie la valeur d'un axe du joystick.
        int getJoyHatsState(int joystick, unsigned int hats); // M�thode qui renvoie la valeur d'une croix multidirectionnelle du joystick (sans r�p�tition).
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

