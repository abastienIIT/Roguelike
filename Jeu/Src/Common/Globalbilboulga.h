#pragma once

#include <FMOD/fmod.hpp>
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

        void clean();
        FMOD_SYSTEM* getAudioSystem();
        SDL_Window* getWindow(void);
        SDL_Renderer* getRenderer(void);
        EventHandler* getEventHandler(void);
        AssetManager* getAssetManager(void) { return assetManager; }
        int getFPS();

        void setAudioSystem(FMOD_SYSTEM *mAudioSystem);
        void setWindow(SDL_Window *mWindow);
        void setRenderer(SDL_Renderer *mRenderer);
        void setEventHandler(EventHandler *mEventHandler);
        void setAssetManager(AssetManager* newAssetManager) { assetManager = newAssetManager; }
        void setFPS(int mFPS);
    private:
        Globalbilboulga();
        ~Globalbilboulga();

        static Globalbilboulga* instance;

        SDL_Window *window;
        SDL_Renderer *renderer;
        EventHandler *eventHandler;

        FMOD_SYSTEM *audioSystem;

        AssetManager* assetManager;

        int FPS;
};
