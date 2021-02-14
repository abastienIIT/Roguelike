#include "../../Game.h"
#include "InputController.h"

void InputController::update()
{
    globalbilboulga->getEventHandler()->updateEvents();

    if (globalbilboulga->getEventHandler()->quit() == true
        || globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_ESCAPE) == true
        || globalbilboulga->getEventHandler()->getJoyButtonState(0, 6))
        {
            Game::isRunning = false;
        }

    if (globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_W) == true
        || globalbilboulga->getEventHandler()->getJoyButtonState(0, 0) == true)
            actions->jumpStart();

    if (globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_A) == true
        || globalbilboulga->getEventHandler()->getJoyHatsState(0, 0) == SDL_HAT_LEFT)
        actions->walk(LEFT);

    if (globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_D) == true
        || globalbilboulga->getEventHandler()->getJoyHatsState(0, 0) == SDL_HAT_RIGHT)
        actions->walk(RIGHT);

    if (globalbilboulga->getEventHandler()->getKeyState(SDL_SCANCODE_SPACE) == true
        || globalbilboulga->getEventHandler()->getJoyButtonState(0, 1) == true)
        actions->swordAttack();
}
