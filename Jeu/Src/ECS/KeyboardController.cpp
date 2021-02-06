#include "../Game.h"
#include "KeyboardController.h"

void KeyboardController::update()
{
	if (Game::event.type == SDL_KEYDOWN)
	{
		switch (Game::event.key.keysym.sym)
		{
		case SDLK_z:
			actions->jumpStart();
			break;

		case SDLK_q:
			actions->walk(LEFT);
			break;

		case SDLK_d:
			actions->walk(RIGHT);
			break;

		case SDLK_SPACE:
			actions->swordAttack();
			break;

		case SDLK_r:
			Game::assets->createEnemies(0, Vector2D(300, 300));
			break;

		default:
			break;
		}
	}

	if (Game::event.type == SDL_KEYUP)
	{
		switch (Game::event.key.keysym.sym)
		{
		case SDLK_q:
			actions->walk(STOP);
			break;

		case SDLK_d:
			actions->walk(STOP);
			break;

		case SDLK_ESCAPE:
			Game::isRunning = false;

		default:
			break;
		}
	}
}
