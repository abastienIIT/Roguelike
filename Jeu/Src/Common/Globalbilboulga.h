#pragma once

#include <FMOD/fmod.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../EventHandler/eventHandler.h"
#include "../AssetManager.h"
#include "../Common/Types/Vector2D.h"
#include "../CharactereCreator.h"
#include "../TrapCreator.h"

class ProjectileCreator;
class ColliderComponent;

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
        TrapCreator* getTrapCreator() { return trapCreator; }
        int getFPS();
        double* getGameSpeed() { return &gameSpeed; }
        Vector2D getCurrentRoomSize();
        int getCurrentTileSize() { return currentTileSize; }
        SDL_Rect* getCamera();
        Vector2D* getWindowSize() { return &windowSize; }
        bool* getDrawAllColliders() { return &drawAllColliders; }
        static const int GRAVITY_STRENGTH = 2;

        void setManager(Manager* newManager) { manager = newManager; }
        void setIsRunning(bool mIsRunning);
        void setAudioSystem(FMOD_SYSTEM *mAudioSystem);
        void setWindow(SDL_Window *mWindow);
        void setRenderer(SDL_Renderer *mRenderer);
        void setEventHandler(EventHandler *mEventHandler);
        void setAssetManager(AssetManager* newAssetManager) { assetManager = newAssetManager; }
        void setProjectileCreator(ProjectileCreator* newPC);
        void setCharactereCreator(CharactereCreator* newCC) { charactereCreator = newCC; }
        void setTrapCreator(TrapCreator* newTC) { trapCreator = newTC; }
        void setFPS(int mFPS);
       // void setGravityStrength(int mgravityStrength);
        void setCurrentRoomSize(Vector2D mCurrentMapSize);
        void setCurrentTileSize(int mCurrentTileSize) { currentTileSize = mCurrentTileSize; }
        void setCamera(SDL_Rect mCamera);
        void setCameraX(int mCameraX);
        void setCameraY(int mCameraY);
        void setCameraW(int mCameraW);
        void setCameraH(int mCameraH);

        std::vector<std::vector<ColliderComponent*>> mapColliders;

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
        TrapCreator* trapCreator;

        bool isRunning;
        int FPS;
        double gameSpeed;

        Vector2D currentRoomSize;
        int currentTileSize = 0;
        SDL_Rect camera;
        Vector2D windowSize;

        bool drawAllColliders = false;

        Manager* manager;
};
