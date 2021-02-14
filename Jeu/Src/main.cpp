#include "Globalbilboulga.h"
#include "Game.h"
#include <stdlib.h>
#include <ctime>

Game* game = nullptr;

int main(int argc, char* argv[])
{
	const int FPSMax = 60;
	const int frameDelay = 1000 / FPSMax;

	Uint32 frameStart;
	int frameTime;

	std::srand((unsigned)std::time(nullptr));

	Globalbilboulga *Globalbilboulga = nullptr;
    Globalbilboulga = Globalbilboulga::getInstance();
    Globalbilboulga->setFPS(60);

    if(Globalbilboulga->init("Jeu", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, 0))
    {
        game = new Game();
        game->init();

        while (Game::isRunning)
        {
            frameStart = SDL_GetTicks();

            game->update();
            game->render();

            frameTime = SDL_GetTicks() - frameStart;
            if (frameTime > 0) Globalbilboulga->setFPS(1000 / frameTime);
            else Globalbilboulga->setFPS(1000);

            if (frameTime < frameDelay)
            {
                SDL_Delay(frameDelay - frameTime);
                Globalbilboulga->setFPS(FPSMax);
            }
        }

        game->clean();
    }
    Globalbilboulga->clean();
	Globalbilboulga::kill();

	return 0;
}
