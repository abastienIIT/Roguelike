#include "../../Game.h"

#include "InputController.h"

void InputController::update()
{

    eventHandler->updateEvents();

    if (eventHandler->quit() == true
        || eventHandler->getKeyState(SDL_SCANCODE_ESCAPE) == true
        || eventHandler->getJoyButtonState(0, 6))
        {
            globalbilboulga->setIsRunning(false);
        }

    if (eventHandler->getKeyState(SDL_SCANCODE_A) == true
        || eventHandler->getJoyHatsState(0, 0) == SDL_HAT_LEFT)
        actions->walk(LEFT);

    if (eventHandler->getKeyState(SDL_SCANCODE_D) == true
        || eventHandler->getJoyHatsState(0, 0) == SDL_HAT_RIGHT)
        actions->walk(RIGHT);

    if (eventHandler->getKeyState(SDL_SCANCODE_SPACE) == true
        || eventHandler->getJoyButtonState(0, 1) == true)
    {
        if (previousSpaceState == false)
        {
            actions->attackPressed();
            previousSpaceState = true;
        }
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_SPACE) == false && previousSpaceState == true
        || eventHandler->getJoyButtonState(0, 1) == true)
    {
        actions->attackRealeased();
        previousSpaceState = false;
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_E) == true
        || eventHandler->getJoyButtonState(0, 2) == true)
    {
        if (previousEState == false)
        {
            actions->attackPressed(true);
            previousEState = true;
        }
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_E) == false && previousEState == true
        || eventHandler->getJoyButtonState(0, 2) == true)
    {
        actions->attackRealeased(true);
        previousEState = false;
    }
    // manage jump button press/release
    if ((eventHandler->getKeyState(SDL_SCANCODE_W) || eventHandler->getJoyButtonState(0, 0)) &&
        previousWState == false)
    {
        actions->jumpStart();
        previousWState = true;
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_W) == false && previousWState == true
        || eventHandler->getJoyButtonState(0, 0) == true)
    {
        actions->jumpStop();
        previousWState = false;
    }
    // --

    if (eventHandler->getKeyState(SDL_SCANCODE_O) == true)
        pause = false;

    if (eventHandler->getKeyState(SDL_SCANCODE_P) == true)
    {
        pauseMode = !pauseMode;
        pause = false;
        std::cout << "Pause mode : " << pauseMode << std::endl;
    }
}
