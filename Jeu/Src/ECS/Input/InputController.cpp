#include "../../Game.h"
#include "InputController.h"

void InputController::update()
{
    Globalbilboulga->getEventHandler()->updateEvents();

    if (Globalbilboulga->getEventHandler()->quit() == true
        || Globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_ESCAPE) == true
        || Globalbilboulga->getEventHandler()->getJoyButtonState(0, 6))
        {
            Game::isRunning = false;
        }

    if (Globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_W) == true
        || Globalbilboulga->getEventHandler()->getJoyButtonState(0, 0) == true)
            actions->jumpStart();

    if (Globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_A) == true
        || Globalbilboulga->getEventHandler()->getJoyHatsState(0, 0) == SDL_HAT_LEFT)
        actions->walk(LEFT);

    if (Globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_D) == true
        || Globalbilboulga->getEventHandler()->getJoyHatsState(0, 0) == SDL_HAT_RIGHT)
        actions->walk(RIGHT);

    if (Globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_SPACE) == true
        || Globalbilboulga->getEventHandler()->getJoyButtonState(0, 1) == true)
        actions->swordAttack();
}
