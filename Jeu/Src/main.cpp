#pragma warning(disable : 26812)

#include <stdlib.h>
#include <ctime>

#include "Common/Globalbilboulga.h"
#include "Game.h"


Game* game = nullptr;

int main(int argc, char* argv[])
{
	const int FPSMax = 60;
	const int frameDelay = 1000 / FPSMax;

	Uint32 frameStart;
	int frameTime;

	std::srand((unsigned)std::time(nullptr));

	Globalbilboulga *globalbilboulga = Globalbilboulga::getInstance();
    game = new Game();
    game->init("Jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0, 32);
    //game->init("Jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, 0, 32);

    while (globalbilboulga->getIsRunning())
    {
        frameStart = SDL_GetTicks();

        game->update();
        //Uint32 updateTime = SDL_GetTicks() - frameStart;
        //std::cout << "Update time : " << updateTime << std::endl;
        game->render();
        //Uint32 renderTime = SDL_GetTicks() - frameStart - updateTime;
        //std::cout << "Render time : " << renderTime << std::endl;

        frameTime = SDL_GetTicks() - frameStart;

        //std::cout << "Total frame time : " << frameTime << " " << frameDelay << std::endl;
        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
            globalbilboulga->setFPS(FPSMax);
        }
        else
        {
            globalbilboulga->setFPS(1000 / frameTime);
        }

        //std::cout << "------------------" << std::endl; //Split each frame in cout
    }

    game->clean();


	return 0;
}
