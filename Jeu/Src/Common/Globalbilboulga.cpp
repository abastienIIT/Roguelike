#include "Globalbilboulga.h"

Globalbilboulga* Globalbilboulga::instance = nullptr;

Globalbilboulga::Globalbilboulga()
{
    window = nullptr;
    renderer = nullptr;
    eventHandler = nullptr;
}

Globalbilboulga::~Globalbilboulga()
{

}

Globalbilboulga *Globalbilboulga::getInstance()
{
    if (instance == nullptr)
    {
        instance = new Globalbilboulga();
    }
    return instance;
}

void Globalbilboulga::kill()
{
    if (instance != nullptr)
    {
        delete(instance);
        instance = nullptr;
    }
}

void Globalbilboulga::clean()
{
    if (eventHandler != nullptr)
    {
        delete(eventHandler);
        eventHandler = nullptr;
    }
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
}

void Globalbilboulga::setWindow(SDL_Window *mWindow)
{
    window = mWindow;
}

SDL_Window* Globalbilboulga::getWindow()
{
    return window;
}

void Globalbilboulga::setRenderer(SDL_Renderer *mRenderer)
{
    renderer = mRenderer;
}

SDL_Renderer* Globalbilboulga::getRenderer()
{
    return renderer;
}

void Globalbilboulga::setEventHandler(EventHandler *mEventHandler)
{
    eventHandler = mEventHandler;
}

EventHandler* Globalbilboulga::getEventHandler()
{
    return eventHandler;
}

void Globalbilboulga::setFPS(int mFPS)
{
    FPS = mFPS;
}

int Globalbilboulga::getFPS()
{
    return FPS;
}
