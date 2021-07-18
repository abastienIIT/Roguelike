#pragma once

#include <FMOD/fmod.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../EventHandler/eventHandler.h"
#include "../AssetManager.h"
#include "../Common/Types/Vector2D.h"
#include "../CharactereCreator.h"

class ProjectileCreator;

class Globalbilboulga
{
    public:
        static Globalbilboulga *getInstance(void);
        static void kill(void);

        void clean();

        Manager* getManager() { return manager; }
        bool getIsRunning();
        FMOD_SYSTEM* getAudioSystem();
        SDL_Window* getWindow(void);
        SDL_Renderer* getRenderer(void);
        EventHandler* getEventHandler(void);
        AssetManager* getAssetManager(void) { return assetManager; }
        ProjectileCreator* getProjectileCreator();
        CharactereCreator* getCharactereCreator() { return charactereCreator; }
        int getFPS();
        int getGravityStrength();
        Vector2D getCurrentRoomSize();
        SDL_Rect* getCamera();
        Vector2D* getWindowSize() { return &windowSize; }

        void setManager(Manager* newManager) { manager = newManager; }
        void setIsRunning(bool mIsRunning);
        void setAudioSystem(FMOD_SYSTEM *mAudioSystem);
        void setWindow(SDL_Window *mWindow);
        void setRenderer(SDL_Renderer *mRenderer);
        void setEventHandler(EventHandler *mEventHandler);
        void setAssetManager(AssetManager* newAssetManager) { assetManager = newAssetManager; }
        void setProjectileCreator(ProjectileCreator* newPC);
        void setCharactereCreator(CharactereCreator* newCC) { charactereCreator = newCC; }
        void setFPS(int mFPS);
        void setGravityStrength(int mgravityStrength);
        void setCurrentRoomSize(Vector2D mCurrentMapSize);
        void setCamera(SDL_Rect mCamera);
        void setCameraX(int mCameraX);
        void setCameraY(int mCameraY);
        void setCameraW(int mCameraW);
        void setCameraH(int mCameraH);

    private:
        Globalbilboulga();
        ~Globalbilboulga();

        static Globalbilboulga* instance;

        SDL_Window *window;
        SDL_Renderer *renderer;
        EventHandler *eventHandler;

        FMOD_SYSTEM *audioSystem;

        AssetManager* assetManager;
        ProjectileCreator* projectileCreator;
        CharactereCreator* charactereCreator;

        bool isRunning;
        int FPS;
        int gravityStrength;
        Vector2D currentRoomSize;
        SDL_Rect camera;
        Vector2D windowSize;

        Manager* manager;
};
