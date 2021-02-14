#include "Globalbilboulga.h"

static Globalbilboulga *myInstanceGlobalbilboulga = nullptr;

Globalbilboulga::Globalbilboulga()
{
    window = nullptr;
    renderer = nullptr;
}

Globalbilboulga::~Globalbilboulga()
{

}

Globalbilboulga *Globalbilboulga::getInstance()
{
    if (myInstanceGlobalbilboulga == nullptr)
    {
        myInstanceGlobalbilboulga = new Globalbilboulga();
    }
    return myInstanceGlobalbilboulga;
}

void Globalbilboulga::kill()
{
    if (myInstanceGlobalbilboulga != nullptr)
    {
        delete(myInstanceGlobalbilboulga);
        myInstanceGlobalbilboulga = nullptr;
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

EventHandler* Globalbilboulga::getEventHandler(void)
{
    return eventHandler;
}