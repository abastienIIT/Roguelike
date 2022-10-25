#include "../../Game.h"

#include "InputController.h"
#include "../../Common/Globalbilboulga.h"

void InputController::init()
{
    eventHandler = Globalbilboulga::getInstance()->getEventHandler();

    actions = &entity->getComponent<ActionsComponent>();
    eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_D);
    eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_W);
    eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_A);
    eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_SPACE);
    eventHandler->setRepeatKeyButton(true, SDL_SCANCODE_E);
    eventHandler->setRepeatJoystickHat(true, 0);

    eventHandler->setRepeatJoystickButton(1, 0);
    eventHandler->setRepeatJoystickButton(1, 3);
    eventHandler->setRepeatJoystickButton(1, 2);
}

void InputController::update()
{

    eventHandler->updateEvents();

    if (eventHandler->quit() == true
        || eventHandler->getKeyState(SDL_SCANCODE_ESCAPE) == true
        || eventHandler->getJoyButtonState(0, 6))
        {
            Globalbilboulga::getInstance()->setIsRunning(false);
        }

    if (eventHandler->getKeyState(SDL_SCANCODE_A) == true
        || eventHandler->getJoyHatsState(0, 0) == SDL_HAT_LEFT)
        actions->walk(LEFT);

    if (eventHandler->getKeyState(SDL_SCANCODE_D) == true
        || eventHandler->getJoyHatsState(0, 0) == SDL_HAT_RIGHT)
        actions->walk(RIGHT);

    if (eventHandler->getKeyState(SDL_SCANCODE_LSHIFT) == true
        || eventHandler->getJoyButtonState(0, 1) == true)
        actions->roll();

    if (eventHandler->getKeyState(SDL_SCANCODE_SPACE) == true
        || eventHandler->getJoyButtonState(0, 2) == true)
    {
        if (previousSpaceState == false)
        {
            actions->attackPressed(0);
            previousSpaceState = true;
        }
    }

    if ((eventHandler->getKeyState(SDL_SCANCODE_SPACE) == false
        && eventHandler->getJoyButtonState(0, 2) == false) && previousSpaceState == true)
    {
        actions->attackRealeased(0);
        previousSpaceState = false;
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_E) == true
        || eventHandler->getJoyButtonState(0, 3) == true)
    {
        if (previousEState == false)
        {
            actions->attackPressed(0, true);
            previousEState = true;
        }
    }

    if ((eventHandler->getKeyState(SDL_SCANCODE_E) == false
        && eventHandler->getJoyButtonState(0, 3) == false) && previousEState == true)
    {
        actions->attackRealeased(0, true);
        previousEState = false;
    }
    // manage jump button press/release
    if ((eventHandler->getKeyState(SDL_SCANCODE_W) || eventHandler->getJoyButtonState(0, 0)) &&
        previousWState == false)
    {
        actions->jumpStart();
        previousWState = true;
    }

    if ((eventHandler->getKeyState(SDL_SCANCODE_W) == false
        && eventHandler->getJoyButtonState(0, 0) == false) && previousWState == true)
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

    if (eventHandler->getKeyState(SDL_SCANCODE_M) == true)
    {
        *Globalbilboulga::getInstance()->getDrawAllColliders() = !*Globalbilboulga::getInstance()->getDrawAllColliders();
        std::cout << "Draw colliders : " << *Globalbilboulga::getInstance()->getDrawAllColliders() << std::endl;
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_SEMICOLON) == true)
    {
        displayMap = true;
        std::cout << "Map displayed" << std::endl;
    }

    if (eventHandler->getKeyState(SDL_SCANCODE_L) == true)
    {
        testButton = true;
        std::cout << "Test button pressed" << std::endl;
    }
}
