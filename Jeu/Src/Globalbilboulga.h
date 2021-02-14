#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ECS/Input/eventHandler.h"

class Globalbilboulga
{
    public:
        static Globalbilboulga *getInstance(void);
        static void kill(void);

        bool init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
        void clean();
        SDL_Window* getWindow(void);
        SDL_Renderer* getRenderer(void);
        EventHandler* getEventHandler(void);
    protected:
    private:
        Globalbilboulga();
        ~Globalbilboulga();

        SDL_Window *window;
        SDL_Renderer *renderer;
        EventHandler *eventHandler;
};
