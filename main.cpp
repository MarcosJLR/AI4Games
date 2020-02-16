#include <SDL.h>
#include <SDL_image.h>

#include "game.hpp"

using namespace std;
using namespace aifg;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 960;

Game* game = NULL;

int main()
{

    const int FPS = 60;
    const int frameDelay = 1000 / FPS;

    int frameStart;
    int deltaTime;

    game = new Game();

    game->init("AIFG", SCREEN_WIDTH, SCREEN_HEIGHT, false);

    while(game->running()){

        frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        deltaTime = SDL_GetTicks() - frameStart;

        if(frameDelay - deltaTime > 0) 
            SDL_Delay(frameDelay - deltaTime);
    }

    return 0;
}