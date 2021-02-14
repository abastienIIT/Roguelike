#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "../EventHandler/eventHandler.h"
#include "../AssetManager.h"

class AssetManager;

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
        AssetManager* getAssetManager(void) { return assetManager; }

        int getFPS();

        void setAssetManager(AssetManager* newAssetManager) { assetManager = newAssetManager; }
        void setFPS(int mFPS);
    private:
        Globalbilboulga();
        ~Globalbilboulga();

        static Globalbilboulga* instance;

        SDL_Window *window;
        SDL_Renderer *renderer;
        EventHandler *eventHandler;

        AssetManager* assetManager;

        int FPS;
};
