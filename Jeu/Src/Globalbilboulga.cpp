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

bool Globalbilboulga::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
    int flags = 0;

	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING))
	{
		return false;
	}
	else
	{
		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	}

	if (TTF_Init() == -1)
	{
		return false;
	}

	eventHandler = new EventHandler();

	return true;
}

void Globalbilboulga::clean()
{
    delete(eventHandler);
    TTF_Quit();
    SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}

SDL_Window* Globalbilboulga::getWindow()
{
    return window;
}

SDL_Renderer* Globalbilboulga::getRenderer()
{
    return renderer;
}

EventHandler* Globalbilboulga::getEventHandler()
{
    return eventHandler;
}

void Globalbilboulga::setFPS(int FPS)
{
    myFPS = FPS;
}

int Globalbilboulga::getFPS()
{
    return myFPS;
}
