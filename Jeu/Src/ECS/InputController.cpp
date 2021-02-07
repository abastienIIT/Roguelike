#include "../Game.h"
#include "InputController.h"

void InputController::update()
{
    updateEvents();

    if (quit() == true) Game::isRunning = false;
    if (getKeyState(SDL_SCANCODE_ESCAPE) == true) Game::isRunning = false;

    if (getKeyState(SDL_SCANCODE_Z) == true) actions->jumpStart();
    if (getKeyState(SDL_SCANCODE_Q) == true) actions->walk(LEFT);
    if (getKeyState(SDL_SCANCODE_D) == true) actions->walk(RIGHT);
    if (getKeyState(SDL_SCANCODE_SPACE) == true) actions->swordAttack();
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

bool InputController::quit(void)
{
    return myQuit;
}
